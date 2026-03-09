/*
 * simple_shell.c
 * A basic Unix shell implementation for my OS course lab 1, 2nd attempt Feb 2026
 * Supports: exit, cd, echo, export (with variable expansion), 
 * background processes (&), and logging background terminations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <pwd.h>

#define MAX_INPUT 1024   // Max characters for user input
#define MAX_ARGS 128     // Max number of arguments/tokens
#define MAX_VARS 64      // Max number of environment variables we can store
#define LOG_FILE "shell_log.txt"

// Struct to hold our custom exported variables
typedef struct {
    char name[64];
    char value[MAX_INPUT];
} ShellVar;

// Global array to store variables for the shell session
static ShellVar vars[MAX_VARS];
static int var_count = 0;

/*
 * Helper function to look up a variable by name.
 * Returns the value if found, or NULL if it doesn't exist.
 */
static const char *get_var(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            return vars[i].value;
        }
    }
    return NULL;
}

/*
 * Adds a new variable or updates an existing one if the user
 * re-exports it.
 */
static void set_var(const char *name, const char *value) {
    // First, check if we just need to update an existing variable
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            strncpy(vars[i].value, value, MAX_INPUT - 1);
            return;
        }
    }
    // If it's new, add it to the end of the array (if we have room)
    if (var_count < MAX_VARS) {
        strncpy(vars[var_count].name, name, 63);
        strncpy(vars[var_count].value, value, MAX_INPUT - 1);
        var_count++;
    }
}

/*
 * Signal handler for when a background child process finishes.
 * Prevents zombie processes.
 */
static void on_child_exit(int sig) {
    (void)sig; // Suppress unused variable warning

    // Note to self: waitpid(-1) waits for ANY child.
    // WNOHANG is super important here so the shell doesn't freeze
    // if a child is still running!
    while (waitpid(-1, NULL, WNOHANG) > 0) {}

    // Log the termination to the file as required by the spec
    FILE *log = fopen(LOG_FILE, "a");
    if (log != NULL) {
        fprintf(log, "Child process was terminated\n");
        fclose(log);
    }
}

/*
 * Registers the signal handler. Using sigaction instead of signal()
 * because the man pages say it handles interrupted system calls better.
 */
static void register_child_signal(void) {
    struct sigaction sa;
    sa.sa_handler = on_child_exit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // Restart functions like scanf if interrupted
    sigaction(SIGCHLD, &sa, NULL);
}

/*
 * Scans a string for '$' and replaces the variable with its actual value.
 */
static void expand_token(const char *src, char *dst, size_t dst_size) {
    size_t di = 0, si = 0;
    size_t src_len = strlen(src);

    while (si < src_len && di < dst_size - 1) {
        if (src[si] == '$') {
            si++; // skip the '$'
            char vname[64]; 
            int vi = 0;

            // Grab the variable name (letters, numbers, underscores)
            while (si < src_len && (src[si] == '_' ||
                   (src[si] >= 'A' && src[si] <= 'Z') ||
                   (src[si] >= 'a' && src[si] <= 'z') ||
                   (src[si] >= '0' && src[si] <= '9'))) {
                vname[vi++] = src[si++];
            }
            vname[vi] = '\0';

            const char *val = get_var(vname);
            if (val == NULL) val = ""; // default to empty string if undefined

            // Copy the expanded value into our destination buffer
            size_t vlen = strlen(val);
            if (di + vlen < dst_size - 1) { 
                memcpy(dst + di, val, vlen); 
                di += vlen; 
            }
        } else {
            dst[di++] = src[si++];
        }
    }
    dst[di] = '\0';
}

/*
 * Parses the raw input string into an array of arguments (argv).
 * Handles quotes and checks for the background symbol '&'.
 */
static int tokenise(char *input, char **argv, int *argc_out) {
    int background = 0, argc = 0;
    char *p = input;

    while (*p != '\0' && argc < MAX_ARGS - 1) {
        // Skip whitespace
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '\0') break;

        if (*p == '"') {
            // Handle quoted arguments (keep them as one token)
            p++;
            char buf[MAX_INPUT]; 
            int bi = 0;
            while (*p != '"' && *p != '\0' && bi < (int)sizeof(buf) - 1) {
                buf[bi++] = *p++;
            }
            buf[bi] = '\0';
            if (*p == '"') p++;

            // Still need to expand variables inside quotes!
            char expanded[MAX_INPUT];
            expand_token(buf, expanded, sizeof(expanded));
            argv[argc++] = strdup(expanded);
        } else {
            // Normal unquoted arguments
            char raw[MAX_INPUT]; 
            int ri = 0;
            while (*p != ' ' && *p != '\t' && *p != '\0' && ri < (int)sizeof(raw) - 1) {
                raw[ri++] = *p++;
            }
            raw[ri] = '\0';

            char expanded[MAX_INPUT];
            expand_token(raw, expanded, sizeof(expanded));

            // If expanding the variable created spaces, split it up again
            char *tok = strtok(expanded, " \t");
            while (tok != NULL && argc < MAX_ARGS - 1) { 
                argv[argc++] = strdup(tok); 
                tok = strtok(NULL, " \t"); 
            }
        }
    }
    argv[argc] = NULL; // execvp requires a NULL terminated array

    // Check if the user wants to run this in the background
    if (argc > 0 && strcmp(argv[argc - 1], "&") == 0) {
        background = 1; 
        free(argv[--argc]); // remove the '&' so we don't pass it to execvp
        argv[argc] = NULL;
    }

    *argc_out = argc;
    return background;
}

// Memory cleanup for the strdup calls in tokenise()
static void free_argv(char **argv) {
    for (int i = 0; argv[i] != NULL; i++) { 
        free(argv[i]); 
        argv[i] = NULL; 
    }
}

/*
 * Built-in command to change directories
 */
static void builtin_cd(char **argv, int argc) {
    const char *target = NULL;

    // Go to home dir if just 'cd' or 'cd ~' is typed
    if (argc < 2 || strcmp(argv[1], "~") == 0) {
        target = getenv("HOME");
        if (target == NULL) { 
            // Fallback if HOME environment variable is missing
            struct passwd *pw = getpwuid(getuid()); 
            target = pw ? pw->pw_dir : "/"; 
        }
    } else { 
        target = argv[1]; 
    }

    if (chdir(target) != 0) {
        perror("cd failed");
    }
}

/*
 * Built-in command for echo
 */
static void builtin_echo(char **argv, int argc) {
    for (int i = 1; i < argc; i++) { 
        if (i > 1) putchar(' '); 
        fputs(argv[i], stdout); 
    }
    putchar('\n');
}

/*
 * Built-in command for export (e.g., export PATH=/bin)
 */
static void builtin_export(char **argv, int argc) {
    if (argc < 2) { 
        fprintf(stderr, "export: missing argument\n"); 
        return; 
    }
    
    // Find the equals sign to split the name and value
    char *eq = strchr(argv[1], '=');
    if (eq == NULL) { 
        fprintf(stderr, "export: invalid syntax (expected name=value)\n"); 
        return; 
    }
    
    *eq = '\0'; // Split the string by replacing '=' with a null terminator
    
    set_var(argv[1], eq + 1);
    setenv(argv[1], eq + 1, 1); // Expose to child processes as well
}

/*
 * Forks the process to run an external command (like ls, cat, grep)
 */
static void execute_command(char **argv, int background) {
    pid_t pid = fork();
    
    if (pid < 0) { 
        perror("fork failed"); 
        return; 
    }
    
    if (pid == 0) {
        // --- Child Process ---
        execvp(argv[0], argv); // Replaces child with the new program
        
        // If execvp returns, it means it couldn't find the command
        fprintf(stderr, "%s: command not found\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    // --- Parent Process ---
    if (!background) { 
        int status; 
        waitpid(pid, &status, 0); // Wait for foreground process to finish
    }
    // If background == 1, we just return to the prompt immediately
}

/*
 * Main infinite loop for the shell
 */
static void shell(void) {
    char input[MAX_INPUT];
    char *argv[MAX_ARGS];
    int argc, background;

    while (1) {
        char cwd[MAX_INPUT];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("[SimpleShell %s]$ ", cwd);
        else
            printf("[SimpleShell]$ ");
        fflush(stdout); // Ensure prompt prints before waiting for input

        // Read user input using scanf.
        // Note: " %[^\n]" skips leading whitespace and reads the full line until newline.
        if (scanf(" %[^\n]", input) == EOF) {
            printf("\n"); // Handle Ctrl+D gracefully
            break;
        }

        if (strlen(input) == 0) continue;

        background = tokenise(input, argv, &argc);
        if (argc == 0) { 
            free_argv(argv); 
            continue; 
        }

        // Check our built-in commands first
        if (strcmp(argv[0], "exit") == 0) {
            free_argv(argv); 
            break;
        } else if (strcmp(argv[0], "cd") == 0) {
            builtin_cd(argv, argc);
        } else if (strcmp(argv[0], "echo") == 0) {
            builtin_echo(argv, argc);
        } else if (strcmp(argv[0], "export") == 0) {
            builtin_export(argv, argc);
        } else {
            // Not a built-in, try to run it as an external program
            execute_command(argv, background);
        }

        // Clean up memory before the next prompt
        free_argv(argv);
    }
}

int main(void) {
    // Setup signal handler for background processes before starting the loop
    register_child_signal();
    shell();
    return 0;
}