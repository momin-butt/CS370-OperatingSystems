#include <stdio.h>
#include <semaphore.h>
#include "part2.h"
#include "main.h"
#include <pthread.h>
#include <stdint.h>

/*
	- DO NOT USE SLEEP FUNCTION
	- Define every helper function in .h file
	- Use Semaphores for synchronization purposes
 */

sem_t traincapacity[50];
sem_t mutex;
sem_t hehe;

int stations, traincap;
int trainatstation[5];

const int INTER_ARRIVAL_TIME = 5;
const int NUM_TRAINS = 5;

// *
//  * Do any initial setup work in this function.
//  * numStations: Total number of stations. Will be >= 5. Assume that initially
//  * the first train is at station 1, the second at 2 and so on.
//  * maxNumPeople: The maximum number of people in a train
pthread_mutex_t lock[50];
void initializeP2(int numStations, int maxNumPeople) {
		
		pthread_t train[5];
		stations = numStations;
		traincap = maxNumPeople;
		sem_init(&mutex, 0, 1); 
		sem_init(&hehe, 0, 1); 
		sem_init(traincapacity, 0,50);
}

void *train(void *i)
{
	sem_wait(&mutex);
	int depart = (long)i;
	trainatstation[depart] = depart;
	sem_post(&mutex);

	int a;
	while(1){
		for(a = 0; a<50; a++){
			sem_wait(&mutex);
			sem_post(&traincapacity[a]);
			sem_post(&mutex);
		}
		sem_wait(&hehe);
		// printf("Train %d moving from station %d to station ", depart, trainatstation[depart]);

		trainatstation[depart] = (trainatstation[depart]+1)%stations;


		sem_post(&hehe);
		// printf("%d.\n", trainatstation[depart]);
	}
	sem_destroy(&traincapacity[a]); 
	sem_destroy(&mutex); 
	sem_destroy(&hehe);

	pthread_exit(0);

}

/**
 * Print in the following format:
 * If a user borads on train 0, from station 0 to station 1, and another boards
 * train 2 from station 2 to station 4, then the output will be
 * 0 0 1
 * 2 2 4
 */
void * goingFromToP2(void * user_data) {

	struct argument *arg = (struct argument*)(user_data); // reading arguments
	int source = arg->from;
	int dest = arg->to;
	int id = arg->id;
	
	int i = 0;

	while(1)
	{

	for(i = 0; i< NUM_TRAINS; i++)
	{

		if(trainatstation[i] == source)
		{
			sem_wait(&traincapacity[i]);
			break;

		}

	}

	if(trainatstation[i] == dest)
	{
		break;
	}
}

	sem_post(&traincapacity[i]);
	printf("%d %d %d\n", id, source, dest);



}

void * startP2(){

	// int i;
	for(int i=0; i<5; i++)
	{
		pthread_t tid[5];
		int id = (intptr_t) i;

		pthread_create(&tid[i], NULL,train, (void *) (intptr_t) i);
		// pthread_join(&tid[i],NULL);
	}


}
