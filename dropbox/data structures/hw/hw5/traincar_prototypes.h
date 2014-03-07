// =======================================================================
//
// IMPORTANT NOTE: You should edit this file
//                 This file is #include'd from traincar.h
//
// =======================================================================
//
// There are a number of additional functions used in main.cpp that
// you need to declare and implement.  Study main.cpp and the provided
// output to determine the arguments, return type, and overall
// behavior.
//
// Add all required additional function prototypes here
// (you may also add your own helper function prototypes here too)
//
// Implement these functions in "traincar.cpp"
//
	

void DeleteAllCars(TrainCar* &car);
void PushBack(TrainCar* &train, TrainCar* new_car);
void CountEnginesAndTotalWeight(TrainCar* train, int &total_weight, int &num_engines, 
								int &num_freight_cars, int &num_passenger_cars, 
								int &num_dining_cars, int &num_sleeping_cars, int extra_weight = 0);
double CalculateSpeed(TrainCar* train, int extra = 0);
float AverageDistanceToDiningCar(TrainCar* &train);
int ClosestEngineToSleeperCar(TrainCar* &train);
std::vector<TrainCar*>  ShipFreight(TrainCar* &all_engines, TrainCar* &all_freight, 
																	int min_speed, int max_cars);
void Separate(TrainCar* &train1, TrainCar* &train2, TrainCar* &train3);