#include <pthread.h>
#include "caltrain.h"

void
station_init(struct station *station)
{
	// initialize the lock and both condition variables
	pthread_mutex_init(&station->lock, NULL);
	pthread_cond_init(&station->train_here, NULL);
	pthread_cond_init(&station->train_can_leave, NULL);

	// no train yet, no one waiting, no one boarding
	station->free_seats = 0;
	station->waiting    = 0;
	station->boarding   = 0;
}

void
station_load_train(struct station *station, int count)
{
	pthread_mutex_lock(&station->lock);

	// tell everyone how many seats this train has
	station->free_seats = count;

	// wake up all waiting passengers so they can try to board
	pthread_cond_broadcast(&station->train_here);

	// wait until the train can actually leave:
	// - no one is still in the middle of boarding, AND
	// - either the train is full OR there are no more passengers waiting
	while (station->boarding > 0 ||
	       (station->free_seats > 0 && station->waiting > 0)) {
		pthread_cond_wait(&station->train_can_leave, &station->lock);
	}

	// reset seats so no one boards after we leave
	station->free_seats = 0;

	pthread_mutex_unlock(&station->lock);
}

void
station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&station->lock);

	// i'm now waiting on the platform
	station->waiting++;

	// sleep until a train is here with at least one free seat
	while (station->free_seats == 0) {
		pthread_cond_wait(&station->train_here, &station->lock);
	}

	// claim my seat before releasing the lock so no one else takes it
	station->free_seats--;
	station->boarding++;
	station->waiting--;

	pthread_mutex_unlock(&station->lock);
	// now i physically walk to my seat (handled outside)
}

void
station_on_board(struct station *station)
{
	pthread_mutex_lock(&station->lock);

	// i'm fully seated now
	station->boarding--;

	// if i was the last one boarding, check if the train can leave
	if (station->boarding == 0 &&
	    (station->free_seats == 0 || station->waiting == 0)) {
		pthread_cond_signal(&station->train_can_leave);
	}

	pthread_mutex_unlock(&station->lock);
}
