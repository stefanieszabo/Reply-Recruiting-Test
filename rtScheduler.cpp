#include "rtScheduler.h"

/*
##########################
RECRUITING TEST
BACKEND ENGINEER INTERNET OF THINGS -- CONCEPT REPLY

Applicant: Stefanie SZABO
Test: Mobility on Demand
Date: 17.09. - 24.09.2018
Task: 3 (Scheduling Service Implementation)

##########################
Task Description
Design and implement a service for scheduling the operation of the cars based on the demands stored in the database from the previous exercises.
The service should respect the desired car features. Furthermore, the service should respect the desired pick-up and drop-off locations as well 
as the earliest pick-up and drop-off times. Also, the service should indicate that a schedule cannot be found in case the constraints cannot be 
satisfied. Otherwise, the service should provide a schedule that minimizes the overall distance travelled by the cars.

##########################
*/

//SCHEDULER
//Well then, let's finish the scheduler.
//Constructors
Scheduler::Scheduler() {

}

Scheduler::Scheduler(std::map<std::string, Car> t_cMap, std::map<std::string, User> t_uMap, std::map<std::string, Demand> t_dMap) {
	this->setCarMap(t_cMap);
	this->setUserMap(t_uMap);
	this->setDemandMap(t_dMap);
}

//set and get functions - those should be self-explanatory
void Scheduler::setCarMap(std::map<std::string, Car> t_carMap) {
	try {
		this->carMap = t_carMap;
	}
	catch (std::exception &e) {
		std::cout << "ERROR: Something went wrong while adding the car map. " << e.what() << "\n";
	}
}
void Scheduler::setUserMap(std::map<std::string, User> t_userMap) {
	try {
		this->userMap = t_userMap;
	}
	catch (std::exception &e) {
		std::cout << "ERROR: Something went wrong while adding the user map. " << e.what() << "\n";
	}
}
void Scheduler::setDemandMap(std::map<std::string, Demand> t_demandMap) {
	try {
		this->demandMap = t_demandMap;
	}
	catch (std::exception &e) {
		std::cout << "ERROR: Something went wrong while adding the demand map. " << e.what() << "\n";
	}
}

std::map<std::string, Car> Scheduler::getCarMap() {
	return this->carMap;
}

std::map<std::string, User> Scheduler::getUserMap() {
	return this->userMap;
}

std::map<std::string, Demand> Scheduler::getDemandMap() {
	return this->demandMap;
}

//pull function for single objects from db tables
Car Scheduler::pullCar(std::map<std::string, Car> t_carMap, std::string t_id) {
	std::map<std::string, Car>::iterator idFinder;
	idFinder = t_carMap.find(t_id);
	try {
		return idFinder->second;
	}
	catch (std::invalid_argument &e) {
		std::cout << "Error: ID is not in the database! " << e.what() << "\n";
	}
}

User Scheduler::pullUser(std::map<std::string, User> t_userMap, std::string t_id) {
	std::map<std::string, User>::iterator idFinder;
	idFinder = t_userMap.find(t_id);
	try {
		return idFinder->second;
	}
	catch (std::invalid_argument &e) {
		std::cout << "Error: ID is not in the database! " << e.what() << "\n";
	}
}

Demand Scheduler::pullDemand(std::map<std::string, Demand> t_demandMap, std::string t_id) {
	std::map<std::string, Demand>::iterator idFinder;
	idFinder = t_demandMap.find(t_id);
	try {
		return idFinder->second;
	}
	catch (std::invalid_argument &e) {
		std::cout << "Error: ID is not in the database! " << e.what() << "\n";
	}
}

//actual scheduling functions

/*int distanceToPickup(Car &t_car, int t_pickup) - calculates car distance to the pick-up location
In order to find the car that is closest to our location and fits the needs of our user, we need to first determine how far the cars are
away from the pick-up location. This will later help us find the optimal car to meet the user's demand.
*/
int Scheduler::distanceToPickup(Car t_car, int t_pickup) {
	if (existsInDB(t_car.getCarID(), this->getCarMap()) == false) {
		throw "ERROR: Car doesn't exist in the DB!\n";
	}
	else {
		return abs(t_car.getCarLoc() - t_pickup);
	}	
}

/*int compareCarDemand(Car &t_car, Demand &t_demand) - compares the car's features to the demanded features and gives weights to the car
Some requests in the demand absolutely have to be fulfilled to make a car optimal - no one wants to travel without half their luggage, leave a person
behind or unable to take their pet or children with them. To make things easy for me, I only check whether these demands can be met by a car.

To this end, I compare the values of the features "number of seats", "luggage space", and "special service options" between car and demand.
Moreover, I check whether the car is available. If the car meets one of these requirements, its weight increases by 1. A car thus can have a maximum
weight of 4. */
int Scheduler::compareCarDemand(Car t_car, Demand t_demand) {
	if (existsInDB(t_car.getCarID(), this->getCarMap()) == false) {
		throw "ERROR: car does not exist in the database!\n";
	}
	else if (existsInDB(t_demand.getDemID(), this->getDemandMap()) == false) {
		throw "ERROR: demand does not exist in the database!\n";
	}//also check if demand is not already in progress, closed or nosched!
	else if (t_demand.getDemStatus() != dStatus::OPEN) {
		throw "ERROR: demand is no longer open!";
	}
	else {
		int carWeight = 0;
		//check if demands are met
		if (t_car.getCOrderStatus() == false) {
			carWeight++;
		}
		if (t_car.getCLugSpace() == t_demand.getDemLuggage()) {
			carWeight++;
		}
		if (t_car.getCNumSeats() == t_demand.getDemNumP()) {
			carWeight++;
		}
		if (t_car.getCSpecialServ() == t_demand.getDemSpecial()) {
			carWeight++;
		}
		return carWeight;
	}
}

/*void scheduleCar(std::map<std::string, Car*>, Demand &t_demand)
To be considered for scheduling, a car must have a weight of 4 and be close to the pickup location - if there is more than one car with these
requirements, the closest car wins. Should there be no car with a weight of 4, the scheduling is interrupted with the demand receiving order status NOSCHED.
If a fitting car is found, its id is added to the demand and the demand status changes to progress.*/
void Scheduler::scheduleCar(Demand &t_demand) {
	//first, check if the demand exists
	if (existsInDB(t_demand.getDemID(), this->getDemandMap() ) == false) {
		throw "ERROR: Demand does not exist in the database!\n";
	}
	//then, check if the demand is not already in progress or could not be scheduled
	else if (t_demand.getDemStatus() == dStatus::PROGRESS || t_demand.getDemStatus() == dStatus::NOSCHED) {
		throw "ERROR: Demand was already scheduled!";
	}
	else {
		std::map<std::string, Car> t_map = this->getCarMap();
		std::string bestCarID;
		//save all cars with weight 4 in a vector
		std::vector<Car> candidateCars;
		std::map<std::string, Car>::iterator mapIT = t_map.begin();
		//find all cars with weight 4
		for (mapIT; mapIT != t_map.end(); mapIT++) {
			int candidateWeight = compareCarDemand(mapIT->second, t_demand);
			if (candidateWeight == 4) {
				candidateCars.push_back(mapIT->second);
			}
		}
		//no car with weight 4 was found.
		if (candidateCars.empty() == true) {
			t_demand.setDemStatus(dStatus::NOSCHED);
			//a message to the user that no scheduled car could be found should be sent. Also, the user's order status is set back to false.
			std::map<std::string, User> t_uMap = this->getUserMap();
			std::map<std::string, User>::iterator userIT = t_uMap.find(t_demand.getDUserID());
			if (userIT == t_uMap.end()) {
				throw "ERROR: User was not found in the database!\n";
			}
			else {
				userIT->second.setUOrderStatus(false);
			}
			//the demand is removed afterwards
			t_demand.removeDemand(t_demand.getDemID(), this->getDemandMap());
		}
		//at least one car with weight 4 was found! Yay!
		//now, calculate all the candidate cars' distances to the pick-up location and find the smallest one
		int minDistance = compareCarDemand(candidateCars[0], t_demand);
		int minIndex = 0;
		for (int i = 0; i < candidateCars.size(); i++) {
			int candDistance = compareCarDemand(candidateCars[i], t_demand);
			if (candDistance < minDistance) {
				minDistance = candDistance;
				minIndex = i;
			}
		}
		//our final candidate should now be the car at index i. Let's get its ID and set all the stuff we need.
		bestCarID = candidateCars[minIndex].getCarID();
		mapIT = t_map.find(bestCarID);
		mapIT->second.setCOrderStatus(true);
		t_demand.setDCarID(bestCarID);
		t_demand.setDemStatus(dStatus::PROGRESS);
		//and finally, update the demand map
		t_demand.updateDemand(this->getDemandMap());
	}	
}

void Scheduler::closeDemand(Demand &t_demand) {
	//check if the demand, car, and user exist
	if (existsInDB(t_demand.getDemID(), this->getDemandMap()) == false) {
		throw "ERROR: Demand does not exist in the DB!\n";
	}
	else if (existsInDB(t_demand.getDCarID(), this->getCarMap()) == false) {
		throw "ERROR: Car does not exist in the DB!\n";
	}
	else if (existsInDB(t_demand.getDUserID(), this->getUserMap()) == false) {
		throw "ERROR: User does not exist in the DB!\n";
	}
	else {
		std::string drivingCarID = t_demand.getDCarID();
		std::string userID = t_demand.getDUserID();
		//find the car and user and check the car's location
		std::map<std::string, Car> t_Cmap = this->getCarMap();
		std::map<std::string, Car>::iterator t_cMapIT = t_Cmap.find(drivingCarID);
		std::map<std::string, User> t_Umap = this->getUserMap();
		std::map<std::string, User>::iterator t_uMapIT = t_Umap.find(userID);
		//when the car has reached its destination, change user, car, and demand statuses
		if (t_cMapIT->second.getCarLoc() == t_demand.getDemDLoc()) {
			t_demand.setDemStatus(dStatus::CLOSED);
			t_uMapIT->second.setUOrderStatus(false);
			t_cMapIT->second.setCOrderStatus(false);

			//and finally, remove the demand
			t_demand.removeDemand(t_demand.getDemID(), this->getDemandMap());
		}
	}
	

	
}