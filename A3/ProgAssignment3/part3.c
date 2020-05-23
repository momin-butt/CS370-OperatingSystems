#include <stdio.h>
#include <semaphore.h>
#include "part3.h"
#include "main.h"
#include <unistd.h>
/*enum DIRECTION {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

enum LANE {
    LEFT,
    RIGHT
};
*/
/*int sem_post (sem_t *sem);*/
/*int sem_wait (sem_t *sem);*/
/*
	- DO NOT USE SLEEP FUNCTION
	- Define every helper function in .h file
	- Use Semaphores for synchronization purposes
 */
sem_t north_bound_l;
sem_t north_bound_r;
sem_t south_bound_l;
sem_t south_bound_r;
sem_t east_bound_l;
sem_t east_bound_r;
sem_t west_bound_l;
sem_t west_bound_r;
sem_t north;
sem_t south;
sem_t east;
sem_t west;
sem_t printLock;
sem_t signal;

int north_lane_left = 0;
int north_lane_right = 0;
int south_lane_left = 0;
int south_lane_right = 0;
int east_lane_left = 0;
int east_lane_right = 0;
int west_lane_left = 0;
int west_lane_right = 0;

int closee = 1;

char* d[] = {"NORTH", "SOUTH", "EAST", "WEST"};
char* l[] = {"LEFT", "RIGHT"};

int n = 0;
int s = 0;
int e = 0;
int w = 0;

void initializeP3() {
    sem_init (&north_bound_l, 0, 0);
    sem_init (&south_bound_l, 0, 0);
    sem_init (&east_bound_l, 0, 0);
    sem_init (&west_bound_l, 0, 0);
    sem_init (&north_bound_r, 0, 0);
    sem_init (&south_bound_r, 0, 0);
    sem_init (&east_bound_r, 0, 0);
    sem_init (&west_bound_r, 0, 0);

    
    sem_init (&north, 0, 1);
    sem_init (&south, 0, 1);
    sem_init (&east, 0, 1);
    sem_init (&west, 0, 1);
    
    sem_init (&printLock, 0, 1);

    sem_init (&signal, 0, 0);
}

/**
 * If there is a car going from SOUTH to NORTH, from lane LEFT,
 * print 
 * SOUTH NORTH LEFT
 * Also, if two cars can simulateneously travel in the two lanes,
 * first print all the cars in the LEFT lane, followed by all the
 * cars in the right lane
 */
void * goingFromToP3(void *argu)
{
    struct argumentP3 *arguP3 = (struct argumentP3*)argu;
    int id;
    int val;
    if(arguP3->from == 0)
    {
        // incrementing lane counter
        sem_wait(&north);
        if(arguP3->lane == 0 /*&& arguP3->to != 2*/)
        {
            north_lane_left ++;
        }
        else if(arguP3->lane == 1)
        {
            north_lane_right ++;
        }
        id = north_lane_left;
        sem_post (&north);  
        // if wants to go left
        if(arguP3->to == 2) // change here
        {
            do
            {
                val = id+n;
                if(val > 1)
                {
                    sem_wait(&north_bound_l);
                }
            }while(closee);
            if(!(val>1))
            {
                sem_wait(&north);
                north_lane_left --;
                n--;
                sem_post (&north);
            }
            sem_wait(&printLock);
            printf("%s %s %s\n", d[arguP3->from], d[arguP3->to], l[arguP3->lane]);
            sem_post(&printLock);
            if(val > 1)
                sem_post(&signal);
            return NULL;
        }
        // if want to go straight, right or left
        else
        {
            if(arguP3->to == 1)
            {
                do
                {
                    sem_wait(&north_bound_l);
                }while(closee);
                sem_wait(&printLock);
                printf("%s %s %s\n", d[arguP3->from], d[arguP3->to], l[arguP3->lane]);
                sem_post(&printLock);
                sem_post(&signal);
                return NULL;
            }
            else if(arguP3->to == 3)
            {
                sem_wait(&north_bound_r);
                sem_wait(&printLock);
                printf("%s %s %s\n", d[arguP3->from], d[arguP3->to], l[arguP3->lane]);
                sem_post(&printLock);
                sem_post(&signal);
                return NULL;
            }

        }

    }
    // south
    if(arguP3->from == 1)
    {
        // incrementing lane counter
        sem_wait(&south);
        if(arguP3->lane == 0 /*&& arguP3->to != 2*/)
        {
            south_lane_left ++;
        }
        else if(arguP3->lane == 1)
        {
            south_lane_right ++;
        }
        id = south_lane_left;
        sem_post (&south);  
        // if wants to go left
        if(arguP3->to == 3) // change here
        {
            do
            {
                val = id+s;
                if(val> 1)
                {
                    sem_wait(&south_bound_l);
                }
            }while(closee);
            if(!(val>1))
            {
                sem_wait(&south);
                south_lane_left --;
                s--;
                sem_post (&south);
            }
            sem_wait(&printLock);
            printf("%s %s %s\n", d[arguP3->from], d[arguP3->to], l[arguP3->lane]);
            sem_post(&printLock);
            if(val> 1)    
                sem_post(&signal);
            return NULL;
        }
        // if want to go straight, right or left
        else
        {
            // if straight
            if(arguP3->to == 0)
            {
                do
                {
                    sem_wait(&south_bound_l);
                }while(closee);
                sem_wait(&printLock);
                printf("%s %s %s\n", d[arguP3->from], d[arguP3->to], l[arguP3->lane]);
                sem_post(&printLock);
                sem_post(&signal);
                return NULL;
            }
            else if(arguP3->to == 2)
            {
                sem_wait(&south_bound_r);
                sem_wait(&printLock);
                printf("%s %s %s\n", d[arguP3->from], d[arguP3->to], l[arguP3->lane]);
                sem_post(&printLock);
                sem_post(&signal);

                return NULL;
            }

        }


    }
    // east
    if(arguP3->from == 2)
    {
         // incrementing lane counter
        sem_wait(&east);
        if(arguP3->lane == 0 /*&& arguP3->to != 2*/)
        {
            east_lane_left ++;
        }
        else if(arguP3->lane == 1)
        {
            east_lane_right ++;
        }
        id = east_lane_left;
        sem_post (&east);  
        // if wants to go left
        if(arguP3->to == 1) // change here
        {
            do
            {
                val = id+e;
                if(val> 1)
                {
                    sem_wait(&east_bound_l);
                }
            }while(closee);
            if(!(val>1))
            {
                sem_wait(&east);
                east_lane_left --;
                e--;
                sem_post (&east);
            }
            sem_wait(&printLock);
            printf("%s %s %s\n", d[arguP3->from], d[arguP3->to], l[arguP3->lane]);
            sem_post(&printLock);
            if(val> 1)    
                sem_post(&signal);
            return NULL;
        }
        // if want to go straight, right or left
        else
        {
            if(arguP3->to == 3)
            {
                do
                {
                    sem_wait(&east_bound_l);
                }while(closee);
                sem_wait(&printLock);
                printf("%s %s %s\n", d[arguP3->from], d[arguP3->to], l[arguP3->lane]);
                sem_post(&printLock);
                sem_post(&signal);
                return NULL;
            }
            else if(arguP3->to == 0)
            {
                sem_wait(&east_bound_r);
                sem_wait(&printLock);
                printf("%s %s %s\n", d[arguP3->from], d[arguP3->to], l[arguP3->lane]);
                sem_post(&printLock);
                sem_post(&signal);
                return NULL;
            }

        }

       
    }
    //west
    if(arguP3->from == 3)
    {
         // incrementing lane counter
        sem_wait(&west);
        if(arguP3->lane == 0 /*&& arguP3->to != 2*/)
        {
            west_lane_left ++;
        }
        else if(arguP3->lane == 1)
        {
            west_lane_right ++;
        }
        id = west_lane_left;
        sem_post (&west);  
        // if wants to go left
        if(arguP3->to == 0) // change here
        {
            do
            {
                val = id+w;
                if(val> 1)
                {
                    sem_wait(&west_bound_l);
                }
            }while(closee);
            if(!(val>1))
            {
                sem_wait(&west);
                west_lane_left --;
                w--;
                sem_post (&west);
            }
            sem_wait(&printLock);
            printf("%s %s %s\n", d[arguP3->from], d[arguP3->to], l[arguP3->lane]);
            sem_post(&printLock);
            if(val > 1)    
                sem_post(&signal);
            return NULL;
        }
        // if want to go straight, right or left
        else
        {
            if(arguP3->to == 2)
            {
                do
                {
                    sem_wait(&west_bound_l);
                }while(closee);
                sem_wait(&printLock);
                printf("%s %s %s\n", d[arguP3->from], d[arguP3->to], l[arguP3->lane]);
                sem_post(&printLock);
                sem_post(&signal);
                return NULL;
            }
            else if(arguP3->to == 1)
            {
                sem_wait(&west_bound_r);
                sem_wait(&printLock);
                printf("%s %s %s\n", d[arguP3->from], d[arguP3->to], l[arguP3->lane]);
                sem_post(&printLock);
                sem_post(&signal);
                return NULL;
            }

        }  
    }
}

void startP3()
{
    int number = 5;
    int i;
    int counter = 0;
    sleep(3);//Done so that all threads of cars are initilized before hand
    /*printf("%d %d\n", north_lane_left, north_lane_right);*/
    /*printf("%d %d\n", south_lane_left, south_lane_right);*/
    /*printf("%d %d\n", east_lane_left, east_lane_right);*/
    /*printf("%d %d\n", west_lane_left, west_lane_right);*/
    while(1)
    {
        if(!(north_lane_right || north_lane_left || south_lane_left || south_lane_right || east_lane_left || east_lane_right || west_lane_left || west_lane_right))
            break;
        //Signal North bound 
        for(i = 0; i < number; i++)
        {
            if(north_lane_left)
            {
                closee = 0;
                sem_post (&north_bound_l);  
                sem_wait(&north);
                north_lane_left --;
                counter++;
                sem_post (&north);
                sem_wait(&signal);
                closee = 1;
            }
            else if(north_lane_right)
            {

                sem_post (&north_bound_r);  
                sem_wait(&north);
                north_lane_right --;
                sem_post (&north);
                sem_wait(&signal);
            }
        }

        sem_wait(&north);
        n -= counter;
        counter = 0;
        for(i = 0; i < north_lane_left;i++)
            sem_post(&north_bound_l);
        sem_post (&north);
        //Signal East bound 
        for(i = 0; i < number; i++)
        {
            if(east_lane_left)
            {
                closee = 0;
                sem_post (&east_bound_l);  
                sem_wait(&east);
                east_lane_left --;
                counter++;
                sem_post (&east);
                sem_wait(&signal);
                closee = 1;
            }
            else if(east_lane_right)
            {

                sem_post (&east_bound_r);  
                sem_wait(&east);
                east_lane_right --;
                sem_post (&east);
                sem_wait(&signal);
            }
        }

        sem_wait(&east);
        e -= counter;
        counter = 0;
        for(i = 0; i < east_lane_left;i++)
            sem_post(&east_bound_l);
        sem_post (&east);
        //Signal South bound 
        for(i = 0; i < number; i++)
        {
            if(south_lane_left)
            {
                closee = 0;
                sem_post (&south_bound_l);  
                sem_wait(&south);
                south_lane_left --;
                counter++;
                sem_post (&south);
                sem_wait(&signal);
                closee = 1;
            }
            else if(south_lane_right)
            {

                sem_post (&south_bound_r);  
                sem_wait(&south);
                south_lane_right --;
                sem_post (&south);
                sem_wait(&signal);
            }
        }

        sem_wait(&south);
        s -= counter;
        counter = 0;
        for(i = 0; i < south_lane_left;i++)
            sem_post(&south_bound_l);
        sem_post (&south);

        //Signal West bound 
        for(i = 0; i < number; i++)
        {
           if(west_lane_left)
           {
                closee = 0;
                sem_post (&west_bound_l);  
                sem_wait(&west);
                west_lane_left --;
                counter++;
                sem_post (&west);
                sem_wait(&signal);
                closee = 1;
           }
           else if(west_lane_right)
           {

                sem_post (&west_bound_r);  
                sem_wait(&west);
                west_lane_right --;
                sem_post (&west);
                sem_wait(&signal);
           }
        }
        sem_wait(&west);
        w -= counter;
        counter = 0;
        for(i = 0; i < west_lane_left;i++)
            sem_post(&west_bound_l);
        sem_post (&west);

    }
}

