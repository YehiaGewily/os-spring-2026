#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Matrix size is at most 20x20 as stated in the lab
#define MAX_SIZE 20

// Global matrices A, B, and C
// I used global so I don't have to pass them to every thread function
int A[MAX_SIZE][MAX_SIZE];
int B[MAX_SIZE][MAX_SIZE];
int C[MAX_SIZE][MAX_SIZE];

// Dimensions of the matrices
int rowA, colA;
int rowB, colB;

// -------------------------------------------------------
// Structs to pass arguments to thread functions
// since pthread only accepts one void* argument
// -------------------------------------------------------

// used in method 2 (per row) - we only need the row number
typedef struct {
  int row;
} RowArgs;

// used in method 3 (per element) - we need both row and column
typedef struct {
  int row;
  int col;
} ElementArgs;

// -------------------------------------------------------
// Function to read a matrix from a file
// -------------------------------------------------------
void read_matrix(const char *filename, int mat[MAX_SIZE][MAX_SIZE], int *rows,
                 int *cols) {
  FILE *file = fopen(filename, "r");

  // check if file opened successfully
  if (file == NULL) {
    printf("Error: could not open file %s\n", filename);
    exit(1);
  }

  // read the first line which contains rows and cols
  // format is: row=x col=y
  fscanf(file, "row=%d col=%d", rows, cols);

  // read all matrix values row by row
  for (int i = 0; i < *rows; i++) {
    for (int j = 0; j < *cols; j++) {
      fscanf(file, "%d", &mat[i][j]);
    }
  }

  fclose(file);
}

// -------------------------------------------------------
// Function to write a matrix to a file
// -------------------------------------------------------
void write_matrix(const char *filename, int mat[MAX_SIZE][MAX_SIZE], int rows,
                  int cols) {
  FILE *file = fopen(filename, "w");

  // check if file opened successfully
  if (file == NULL) {
    printf("Error: could not open file %s\n", filename);
    exit(1);
  }

  // write the header line first
  fprintf(file, "row=%d col=%d\n", rows, cols);

  // write each row of the matrix
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      // add space between numbers but not at the end of the line
      if (j == 0)
        fprintf(file, "%d", mat[i][j]);
      else
        fprintf(file, " %d", mat[i][j]);
    }
    fprintf(file, "\n");
  }

  fclose(file);
}

// -------------------------------------------------------
// METHOD 1: A thread per matrix
// One single thread does all the multiplication
// -------------------------------------------------------
void *compute_whole_matrix(void *arg) {
  // loop through every element in the output matrix C
  for (int i = 0; i < rowA; i++) {
    for (int j = 0; j < colB; j++) {
      C[i][j] = 0;
      // dot product of row i from A and column j from B
      for (int k = 0; k < colA; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  return NULL;
}

// -------------------------------------------------------
// METHOD 2: A thread per row
// Each thread handles one full row of the output matrix C
// -------------------------------------------------------
void *compute_row(void *arg) {
  // cast the argument back to RowArgs struct
  RowArgs *args = (RowArgs *)arg;
  int i = args->row;

  // free the memory we allocated for args (no memory leaks!)
  free(args);

  // compute all columns for this row
  for (int j = 0; j < colB; j++) {
    C[i][j] = 0;
    for (int k = 0; k < colA; k++) {
      C[i][j] += A[i][k] * B[k][j];
    }
  }
  return NULL;
}

// -------------------------------------------------------
// METHOD 3: A thread per element
// Each thread computes just one single element of matrix C
// -------------------------------------------------------
void *compute_element(void *arg) {
  // cast the argument back to ElementArgs struct
  ElementArgs *args = (ElementArgs *)arg;
  int i = args->row;
  int j = args->col;

  // free the memory we allocated for args (no memory leaks!)
  free(args);

  // compute the single element C[i][j]
  C[i][j] = 0;
  for (int k = 0; k < colA; k++) {
    C[i][j] += A[i][k] * B[k][j];
  }
  return NULL;
}

// -------------------------------------------------------
// MAIN
// -------------------------------------------------------
int main(int argc, char *argv[]) {

  // default file names
  char fileA[200] = "a.txt";
  char fileB[200] = "b.txt";
  char outPrefix[200] = "c";

  // if the user passed arguments, use them instead
  if (argc == 4) {
    sprintf(fileA, "%s.txt", argv[1]);
    sprintf(fileB, "%s.txt", argv[2]);
    sprintf(outPrefix, "%s", argv[3]);
  }

  // build output file names using the prefix
  char out_matrix[300], out_row[300], out_element[300];
  sprintf(out_matrix, "%s_per_matrix.txt", outPrefix);
  sprintf(out_row, "%s_per_row.txt", outPrefix);
  sprintf(out_element, "%s_per_element.txt", outPrefix);

  // read the two input matrices from files
  read_matrix(fileA, A, &rowA, &colA);
  read_matrix(fileB, B, &rowB, &colB);

  // make sure dimensions are compatible for multiplication
  // A is (rowA x colA) and B is (rowB x colB)
  // we need colA == rowB for multiplication to work
  if (colA != rowB) {
    printf("Error: matrix dimensions don't match! colA=%d but rowB=%d\n", colA,
           rowB);
    return 1;
  }

  // variables to measure time
  struct timeval start, stop;
  long seconds, microseconds;

  // ==================================================
  // METHOD 1: one thread for the whole matrix
  // ==================================================
  pthread_t thread1;

  gettimeofday(&start, NULL); // start timer

  pthread_create(&thread1, NULL, compute_whole_matrix, NULL);
  pthread_join(thread1, NULL); // wait for thread to finish

  gettimeofday(&stop, NULL); // stop timer

  // calculate time taken
  seconds = stop.tv_sec - start.tv_sec;
  microseconds = stop.tv_usec - start.tv_usec;

  // write result to file
  write_matrix(out_matrix, C, rowA, colB);

  printf("Method 1 (per matrix):  Threads = 1,   Time = %ld seconds %ld "
         "microseconds\n",
         seconds, microseconds);

  // ==================================================
  // METHOD 2: one thread per row
  // we create rowA threads, one for each row
  // ==================================================
  pthread_t row_threads[MAX_SIZE];

  gettimeofday(&start, NULL); // start timer

  // create all threads first before joining any of them
  // (joining immediately after create would be sequential, not parallel)
  for (int i = 0; i < rowA; i++) {
    // allocate args on the heap so it stays alive when thread runs
    RowArgs *args = malloc(sizeof(RowArgs));
    args->row = i;
    pthread_create(&row_threads[i], NULL, compute_row, args);
  }

  // now wait for all threads to finish
  for (int i = 0; i < rowA; i++) {
    pthread_join(row_threads[i], NULL);
  }

  gettimeofday(&stop, NULL); // stop timer

  seconds = stop.tv_sec - start.tv_sec;
  microseconds = stop.tv_usec - start.tv_usec;

  // write result to file
  write_matrix(out_row, C, rowA, colB);

  printf("Method 2 (per row):     Threads = %d,   Time = %ld seconds %ld "
         "microseconds\n",
         rowA, seconds, microseconds);

  // ==================================================
  // METHOD 3: one thread per element
  // we create rowA * colB threads, one for each element
  // ==================================================

  // total number of threads = number of elements in C
  int total_threads = rowA * colB;
  pthread_t element_threads[MAX_SIZE * MAX_SIZE];

  gettimeofday(&start, NULL); // start timer

  // create all threads first
  int t = 0; // thread index
  for (int i = 0; i < rowA; i++) {
    for (int j = 0; j < colB; j++) {
      // allocate args on the heap so it stays alive when thread runs
      ElementArgs *args = malloc(sizeof(ElementArgs));
      args->row = i;
      args->col = j;
      pthread_create(&element_threads[t], NULL, compute_element, args);
      t++;
    }
  }

  // now wait for all threads to finish
  for (int i = 0; i < total_threads; i++) {
    pthread_join(element_threads[i], NULL);
  }

  gettimeofday(&stop, NULL); // stop timer

  seconds = stop.tv_sec - start.tv_sec;
  microseconds = stop.tv_usec - start.tv_usec;

  // write result to file
  write_matrix(out_element, C, rowA, colB);

  printf("Method 3 (per element): Threads = %d,  Time = %ld seconds %ld "
         "microseconds\n",
         total_threads, seconds, microseconds);

  return 0;
}