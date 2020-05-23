#ifndef PART_1
#define PART_1


struct Elevator{

	int* start;
	int* end;
	int elevatorCap;

};

void initializeP1(int numFloors, int maxNumPeople);
void *goingFromToP1(void *);
void startP1();
void* myElevator();

#endif
