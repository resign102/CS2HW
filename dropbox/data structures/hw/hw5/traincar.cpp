// =======================================================================
//
// IMPORTANT NOTE: You should edit this file
//
//    This file provides the code for ASCII art printing of trains.
//    You should implement all of the functions prototyped in
//    "traincar_prototypes.h" in this file.
//
// =======================================================================


#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <cmath>
#include <vector>
#include <cstdlib>
#include "traincar.h"


// =======================================================================
// =======================================================================

// This helper function checks that the forward and backward pointers
// in a doubly-linked structure are correctly and consistently assigned.
void SanityCheck(TrainCar* train) {
  // an empty train is valid
  if (train == NULL) return;
  assert (train->prev == NULL);
  TrainCar *tmp = train;
  while (tmp->next != NULL) {
    // the next train better point back to me
    assert (tmp->next->prev == tmp);
    tmp = tmp->next;
  }
}


// This helper function prints one of the 5 rows of the TrainCar ASCII art
void PrintHelper(TrainCar* t, int which_row) {
  if (t == NULL) {
    // end of the line
    std::cout << std::endl;
    return;
  }

  if (which_row == 0) {
    // the top row only contains "smoke" for engine traincars
    if (t->isEngine()) {
      std::cout << "     ~~~~";
    } else {
      std::cout << "         ";
    }
  } else if (which_row == 1) {
    // the 2nd row only contains the smoke stack for engine traincars
    if (t->isEngine()) {
      std::cout << "    ||   ";
    } else {
      std::cout << "         ";
    }
  } else if (which_row == 2) {
    // the 3rd row contains the ID for each traincar
    // (and engine traincars are shaped a little differently)
    if (t->isEngine()) {
      std::cout << "   " << std::setw(6) << std::setfill('-') << t->getID();
    } else {
      std::cout << std::setw(9) << std::setfill('-') << t->getID();
    }
    std::cout << std::setfill(' ');
  } else if (which_row == 3) {
    // the 4th row is different for each TrainCar type
    if (t->isEngine()) {
      std::cout << " / ENGINE";
    } else if (t->isFreightCar()) {
      // freight cars display their weight
      std::cout << "|" << std::setw(5) << t->getWeight() << "  |";
    } else if (t->isPassengerCar()) {
      // passenger cars are simple empty boxes
      std::cout << "|       |";
    } else if (t->isDiningCar()) {
      std::cout << "|  dine |";
    } else {
      assert (t->isSleepingCar());
      std::cout << "| sleep |";
    }
  } else if (which_row == 4) {
    // final row is the same for all cars, just draw the wheels
    std::cout << "-oo---oo-";
  }

  // between cars display the '+' link symbol on the 5th row 
  // (only if there is a next car)
  if (t->next != NULL) {
    if (which_row == 4) {
      std::cout << " + ";
    } else {
      std::cout << "   ";
    }
  }

  // recurse to print the rest of the row
  PrintHelper(t->next, which_row);
}


void PrintTrain(TrainCar* train) {
  
  if (train == NULL) { 
    std::cout << "PrintTrain: empty train!" << std::endl; 
    return; 
  }

  // Print each of the 5 rows of the TrainCar ASCII art
  PrintHelper(train, 0);
  PrintHelper(train, 1);
  PrintHelper(train, 2);
  PrintHelper(train, 3);
  PrintHelper(train, 4);

  
  // UNCOMMENT THESE ADDITIONAL STATISTICS AS YOU WORK

  int total_weight = 0;
  int num_engines = 0;
  int num_freight_cars = 0;
  int num_passenger_cars = 0;
  int num_dining_cars = 0;
  int num_sleeping_cars = 0;
  CountEnginesAndTotalWeight
    (train, total_weight,
     num_engines, num_freight_cars, num_passenger_cars, num_dining_cars, num_sleeping_cars);
  int total_cars = num_engines+num_freight_cars+num_passenger_cars+num_dining_cars+num_sleeping_cars;
  float speed = CalculateSpeed(train);
  std::cout << "#cars = " << total_cars;
  std::cout << ", total weight = " << total_weight;
  std::cout << ", speed on 2% incline = " << std::setprecision(1) << std::fixed << speed;
  
  // If there is at least one passenger car, print the average
  // distance to dining car statistic
  if (num_passenger_cars > 0) {
    float dist_to_dining = AverageDistanceToDiningCar(train);
    if (dist_to_dining < 0) {
      // If one or more passenger cars are blocked from accessing the
      // dining car (by an engine car) then the distance is infinity!
      std::cout << ", avg distance to dining = inf";
    } else {
      std::cout << ", avg distance to dining = " << std::setprecision(1) << std::fixed << dist_to_dining;
    }
  }

  // If there is at least one sleeping car, print the closest engine
  // to sleeper car statistic
  if (num_sleeping_cars > 0) {
    int closest_engine_to_sleeper = ClosestEngineToSleeperCar(train);
    std::cout << ", closest engine to sleeper = " << closest_engine_to_sleeper;
  }

  std::cout << std::endl;
  
}


// =======================================================================
//	STUDENT IMPLEMENTED FUNCTIONS BELOW
// 
// USEFUL HELPER LINES OF CODE ARE COMMENTED
// OUT BELOW FOR EASE & FUTURE USE.
// =======================================================================

void DeleteAllCars(TrainCar* &car) {
	while (car) {
		TrainCar* old = car;
		car = car->next;
		delete old;
	}	
}

// static TrainCar* MakeFreightCar(int weight) { return new TrainCar('f',weight); }
void PushBack(TrainCar* &train, TrainCar* new_car) {
	if (train == NULL) {				//error handling for an empty train
		train = new_car;				//point the train head to the first car
		new_car->next = NULL; 		//add the new car to the end of the train
		new_car->prev = NULL;		//no prev pointer since this is the first car
	}
	
	else {								
		TrainCar* current = train;	//otherwise, just find the last car
		while (current->next != NULL) 
			{ current = current->next;	}
		new_car->next = NULL; 		//add the new car to the end of the train
		new_car->prev = current;	//point back to the old caboose
		current->next = new_car;	//point the old caboose to the new one
	}
	
}

void CountEnginesAndTotalWeight(TrainCar* train, int &total_weight, int &num_engines, 
									int &num_freight_cars, int &num_passenger_cars, 
									int &num_dining_cars, int &num_sleeping_cars, int extra_weight) {
	total_weight = 0;
	num_engines = 0;
	num_freight_cars = 0;
	num_passenger_cars = 0;
	num_dining_cars = 0;
	num_sleeping_cars = 0;
	TrainCar* current = train;
	while (current) {
		//basic counting loop
		if (current->isEngine()) {
			num_engines += 1;
			total_weight += current->getWeight();
		} else if (current->isFreightCar()) {
			num_freight_cars += 1;
			total_weight += current->getWeight();
		} else if (current->isPassengerCar()) {
			num_passenger_cars += 1;
			total_weight += current->getWeight();
		} else if (current->isDiningCar()) {
			num_dining_cars += 1;
			total_weight += current->getWeight();
		} else if (current->isSleepingCar()) {
			num_sleeping_cars += 1;
			total_weight += current->getWeight();
		}
		current = current->next;
	}
	total_weight += extra_weight;
}

double CalculateSpeed(TrainCar* train, int extra) {
	int total_weight = 0;
	int num_engines = 0;
	int num_freight_cars = 0;
	int num_passenger_cars = 0;
	int num_dining_cars = 0;
	int num_sleeping_cars = 0;
	//grab some numbers
	CountEnginesAndTotalWeight(train, total_weight, num_engines, 
                           num_freight_cars, num_passenger_cars,
                           num_dining_cars, num_sleeping_cars, extra);
	//given equation. Doubles! Longs! It's the future!
	double speed = ( (num_engines * double(3000)) * double(550) *  double(3600) ) 
						/ ( (20/.01) * .02 * 5280 * total_weight );
	//speed = floor(speed * 10.0f) / 10.0f;	//truncate our float
	return speed;
}

float AverageDistanceToDiningCar(TrainCar* &train) {
	bool trapped = true; 		//watch for inaccessible passenger cars
	TrainCar* current = train;
	float total_cars = 0;		//keep track of the total number of passenger cars
	float total_distance = 0;	//we'll use this to hold the average distance measurements
	int current_high = 0;		//count the furthest distance away from a dining car
	while (current->next != NULL) {
		if (current->isPassengerCar() == false) {
			if (current->isDiningCar() == false && current_high > 0) //algorithm to determine
				trapped = true;													//trapped passengers
			else 
				trapped = false;
			if (trapped == true)
				return -1.0; 		//a num < 0 returns inf avg distance.
			for (int i = 0; current_high != 0; ++i) {	//if a car is 3 away from a dining car
				total_distance += current_high;			//then that means one car is two away and
				--current_high;								//one car is one away, logically.
			}
		}
		
		else {	//otherwise, we just keep counting
			total_cars += 1;
			current_high += 1;
		}
		current = current->next;
	}
	
	if (current->isPassengerCar()) { //error check: if the last car is a passenger car,
		total_cars += 1;					//make sure to include it in the count and then 
		current_high += 1;				//run through our totals addition loop
	}
	
	for (int i = 0; current_high != 0; ++i) {	//if a car is 3 away from a dining car,	
		total_distance += current_high;			//then that means one car is two away and
		--current_high;								//one car is one away, logically.
	}
	
	float ave = total_distance / total_cars;
	return ave;
}

int ClosestEngineToSleeperCar(TrainCar* &train){
	TrainCar* current = train;
	bool first = true; 	//error checking for non-existent current_low
	int curr = 0;
	int current_low;
	while (current) {
		//count how many cars have passed until we hit an engine or a sleeper car,
		//then compare the number to our current lowest number, and if its higher, 
		//make it the new current low.
		if (current->isSleepingCar() == true || current->isEngine() == true) {
			if (first == true) {
				current_low = curr;
				curr = 1;
				first = false;
			}
			else if (curr > current_low) {
				current_low = curr;
				curr = 1;
			}
		}
		else
			curr += 1;
		current = current->next;
	}
	return current_low;
}

std::vector<TrainCar*> ShipFreight(TrainCar* &all_engines, TrainCar* &all_freight, int min_speed, int max_cars){
//all_engines = collection of engines
//all_freight = collection of freight cars
//min_speed = min speed for train
//max_cars = max cars per train
	
	std::vector<TrainCar*> trains;	//current train-head	
	TrainCar* current_train = NULL;	//nothing yet
	TrainCar *freight_car;				//for moving freight
	int total = 0;							//count the cars/train
	TrainCar* test;
	
	while (all_freight) {							//while we have freight cars,
		if (current_train == NULL) {				//if we have no train yet,
			if(all_engines) {							//and we have engines,
				current_train = all_engines;		//our train is our first engine
				test = current_train;
				all_engines = all_engines->next;	//move our engines forward
				current_train->next = NULL;		//and we make our NULL end
				current_train->prev = NULL;
				if (all_engines)						//since we move our engine,
					all_engines->prev = NULL;		//check to see if have more
				total += 1;								//and add a car
				//std::cout << "STARTED TRAIN " << total << std::endl;
			}
		}
		
		if (total < max_cars) { //if we can fit more cars,
			/*///
			std::cout << "SPEED "<<
			CalculateSpeed(current_train, all_freight->getWeight()) << " GETWEIGHT "
			<<all_freight->getWeight()<<std::endl;
			/*///
			if (CalculateSpeed(current_train, all_freight->getWeight()) > min_speed) {
			//Calculate speed has been modified to include an additional
			//argument "extra weight" that the professor did not initially
			//include. It is defaulted to 0, and is used only in addition
			//operations, rendering it useless unless explicitly called.
			//It is used to check if the additional weight of the next
			//freight car will break the min_speed limit.
			
				freight_car = all_freight;
				all_freight = all_freight->next;
				freight_car->next = NULL;
				if (all_freight)
					all_freight->prev=NULL;
					
				//add our current freight car
				TrainCar* current = test;	
				while (current->next != NULL) 
					{ current = current->next;	}
				freight_car->next = NULL; 		
				freight_car->prev = current;	
				current->next = freight_car;
				
				total += 1;
				//std::cout << "ADD FREIGHT " << total << std::endl;
			}
			else { //send the old train away and re-initialize
				//std::cout << "PUSH BACK" << std::endl;
				trains.push_back(current_train);
				current_train = NULL;
				total = 0;
			}
		}
		
		else { //re-initialize here, too (same conitions are broken)
			//std::cout << "PUSH BACK" << std::endl;
			trains.push_back(current_train);
			current_train = NULL;
			total = 0;
		}
	}
	trains.push_back(current_train);	//finish our current train since were out of freight
	current_train = NULL;
	return trains;
}

void Separate(TrainCar* &train1, TrainCar* &train2, TrainCar* &train3) {
	int total = 0;						//total cars
	int count = 0;						//a dynamic counter
	TrainCar* test1 = train1;		
	TrainCar* test2 = train1;		//these three pointers serve as re-usable iterators through 
	TrainCar* test3 = train1;		//the train, to prevent having to loop back to the start
	bool first_half_engine = false;	
	bool second_half_engine = false;

	while (test1) { total += 1; 
						 test1 = test1->next; } 
						
//////////////////////////////////////////////////////////////////////////					
///////////// DETERMINE IF TRAIN CAN BE SPLIT IN TWO DOWN THE MIDDLE /////
//////////////////////////////////////////////////////////////////////////
	
	//try to find an engine on each half of the train
	while(test2) {																			
		if ( count < (total / 2) ) {													
			if (test2->isEngine()) 	{						
				//std::cout << " FOUND LEFT ENGINE " << std::endl;				
				first_half_engine = true;	
			}
		}																						
		else if ( count >= (total / 2) ) {											
			if (test2->isEngine()) {										
				//std::cout << " FOUND RIGHT ENGINE " << std::endl;			
				second_half_engine = true;			
			}
		}																						
		count += 1;																			
		test2 = test2->next;																
	}																							

	count = 0;	
	//if we have >0 engines on both sides,
	if (first_half_engine == true && second_half_engine == true) {			
		//std::cout << "SPLITTING DOWN THE MIDDLE" << std::endl;
		train2 = train1;																	
		while(train2) {																	
			if (count < (total/2)) 													
				train2 = train2->next;			//find the middle of the train,						
			else {																				
				train3 = train2;					//assign our new trains,
				train2 = train1;					
				train1 = NULL;						//erase our initial train,
				train3->prev->next = NULL;		//and affix our new trains	
				train3->prev = NULL;				//to their newly split halves	
				break;																			
			}																					
			count += 1;																		
		}																						
	}
//////////////////////////////////////////////////////////////////////////
///////////////////TRAIN CANNOT BE SPLIT, RE-ARRANGE ENGINES//////////////
//////////////////////////////////////////////////////////////////////////
	else {
		// if we have engines on only the right side, we just need to slap 
		// one of them on the front of our train!
		if (second_half_engine == true) {
			while(test3) {
				if ( count > (total / 2) ) {
					if (test3->isEngine()) {
						//std::cout << "found engine on LEFT"<<std::endl;
						test3->next->prev = test3->prev;		//de-link the engine
						test3->prev->next = test3->next;		
						test3->next = train1;					// link the engine back 
						test3->prev = NULL;						// to the front and 
						test3->next->prev = test3;				// re-assign it as the lead car
						train1 = test3;
						break;
					}
				}
				count += 1;
				test3 = test3->next;
			}
			train2 = train1;
			count = 0;
			while(train2) {				// then continue with the 
				if (count < (total/2))	// splitting process as normal
					train2 = train2->next;
				else {
					train3 = train2;	
					train2 = train1;	
					train1 = NULL;		
					train3->prev->next = NULL;	
					train3->prev = NULL;	
					break;
				}
				count += 1;	
			}
		}
//////////////////////////////////////////////////////////////////////////
///////////////////////////ALL ENGINES ON LEFT SIDE///////////////////////
//////////////////////////////////////////////////////////////////////////
		else if (first_half_engine == true) {
			while (train1->next != NULL) { train1 = train1->next; } 
			// if we have engines on only the left side, we 
			// need to put one at the end of our second train
			while(test3->next != NULL) {
				if ( count < (total / 2) ) {
					if (test3->isEngine()) {
						if (test3->prev == NULL) { //engine is the first car
							test3->next->prev = NULL;
							test3->prev = train1;
							train1->next = test3;
							test3->next = NULL;
							break;
						}
						else {
							test3->next->prev = test3->prev;
							test3->prev->next = test3->next;
							train1->next = test3;
							test3->next = NULL;
							test3->prev = train1;
							break;
						}	
					}
				}
				count += 1;
				test3 = test3->next;
			}
			
			//push train1 back to its front;
			while(train1->prev != NULL) { train1 = train1->prev; }
			
			train2 = train1;
			count = 0;
			while(train2) {				// then continue with the 
				if (count < (total/2))	// splitting process as normal
					train2 = train2->next;
				else {
					train3 = train2;	
					train2 = train1;	
					train1 = NULL;		
					train3->prev->next = NULL;	
					train3->prev = NULL;	
					break;
				}
				count += 1;	
			}
		}
//////////////////////////////////////////////////////////////////////////
	}
}

