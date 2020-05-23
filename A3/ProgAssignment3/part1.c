#include "part1.h"
#include <pthread.h>
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

/**
 * Do any initial setup work in this function.
 * numFloors: Total number of floors elevator can go to
 * maxNumPeople: The maximum capacity of the elevator
 */

int people;
int floorsnum = -1;
int thisFloor = 0;

sem_t* GoingTo;
sem_t* GoingFrom;
sem_t mutex;

struct Elevator elevator;

void initializeP1(int numFloors, int maxNumPeople) {

	GoingFrom = malloc(sizeof(sem_t)*numFloors);
	GoingTo = malloc(sizeof(sem_t)*numFloors);	

	people = maxNumPeople;
	floorsnum = numFloors+1;

	sem_init(&mutex,0,1);
	sem_init(GoingFrom,0,1);
	sem_init(GoingTo,0,1);

	elevator.start = malloc(sizeof(int)*numFloors);
	elevator.end = malloc(sizeof(int)*numFloors);
}


/**
 * This program should print data in the specific format
 * described below:
 * If there is a passenger (id 5) going from floor 2 to 4, followed
 * by a passenger (id 6) from 2 to 5, the output should be:
 * 5 2 4
 * 6 2 5
 * i.e. there should be a single space, and each journey should be 
 * on a new line. Incorrectly formatted output will get 0 marks
 */
void* goingFromToP1(void *arg) {
	// sem_wait(&mutex);
    struct argument *elevargs = (struct argument *) arg;

	int id = elevargs->id;
	int src = elevargs->from;
	int dest  = elevargs->to;

	sem_wait(&mutex);

	elevator.start[dest] = elevator.start[dest]+1;
	elevator.end[src] = elevator.end[src]+1;

	sem_post(&mutex);

	sem_wait(&GoingFrom[src]);

	sem_wait(&GoingTo[dest]);
	sem_post(&GoingTo[dest]);

	sem_post(&GoingFrom[src]);

	printf("%d %d %d\n", id,src,dest);

}

void* myElevator()

{	
	sem_wait(&mutex);
	
	bool elevMoving = true;
	bool found = true;

	sem_post(&mutex);

	while(true){

		int i,j;

		for(i=0;i<255;i++)
			for(j=0;j<255;j++);

		while(elevator.start[thisFloor] == found)
		{
			sem_wait(&mutex);
			elevator.start[thisFloor] = elevator.start[thisFloor] - 1;
			elevator.elevatorCap = elevator.elevatorCap - 1;
			sem_post(&mutex);

			sem_wait(&mutex);
			sem_post(&GoingTo[thisFloor]);
			sem_post(&mutex);
		}
		
		while(people > elevator.elevatorCap  && elevator.end[thisFloor] == found)
		{
			sem_wait(&mutex);
			elevator.elevatorCap = elevator.elevatorCap + 1;
			sem_post(&mutex);

			sem_wait(&mutex);
			elevator.end[thisFloor] = elevator.end[thisFloor] - 1;
			sem_post(&mutex);

			sem_wait(&mutex);
			sem_post(&GoingFrom[thisFloor]);
			sem_post(&mutex);
			
		}


		if(elevMoving == true)
		{
			sem_wait(&mutex);
			thisFloor = thisFloor + 1;
			sem_post(&mutex);			
		}

		else
		{
			sem_wait(&mutex);
			thisFloor = thisFloor - 1;
			sem_post(&mutex);
		}
		
	}
		
	pthread_exit(0);

}

void startP1(){
	
		sleep(0.75);

		pthread_t myelev1;
		pthread_create(&myelev1, NULL, myElevator, NULL);
		
		pthread_t myelev2;
		pthread_create(&myelev2, NULL, myElevator, NULL);

		// pthread_join(myelev1,NULL);
		// pthread_join(myelev2,NULL);

		// free(elevator.start);
		// free(elevator.end);
	
}


