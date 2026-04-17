#include <pthread.h>

struct station {
	pthread_mutex_t lock;           // one lock to protect everything
	pthread_cond_t train_here;      // passengers wait here until a train shows up
	pthread_cond_t train_can_leave; // train waits here until everyone is seated
	int free_seats;  // seats left on the current train (0 when no train is here)
	int waiting;     // how many passengers are waiting on the platform
	int boarding;    // passengers who got a seat but haven't called on_board yet
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
