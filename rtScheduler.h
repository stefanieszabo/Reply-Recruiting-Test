#pragma once
#include "rtMgmtDemand.h"
#include<vector>

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
Design and implement a service for managing the data stored in the database from the previ-ous exercise, i.e. cars, users, and demands.

The service should support adding and removing cars as well as changing the car details. In particular, it should be possible to
update the current location of the car independently. Furthermore, the service should support adding and removing users as well as
changing the user details. Finally, the service should support adding and removing demands as well as changing demand details.
##########################
*/

//Scheduling service

/*The scheduler serves as the part of our mobility on demand service that creates a demand based on user input and looks for the optimal car to fulfill the demand.
To this end, we need some functions:
- a function that calculates the distance between cars and the user's desired pickup location
- a function that compares cars to demands
- a function that chooses a car and assigns it to a demand
- and finally, a function that closes the demand after the car has reached the drop-off location or if a scheduling was not possible.
*/

class Scheduler{
private:
std::map<std::string, Car> carMap;
std::map<std::string, User> userMap;
std::map<std::string, Demand> demandMap;

public:
Scheduler();
Scheduler(std::map<std::string, Car> t_cMap, std::map<std::string, User> t_uMap, std::map<std::string, Demand> t_dMap);
//set and get functions for the maps
void setCarMap(std::map<std::string, Car> t_carMap);
void setUserMap(std::map<std::string, User> t_userMap);
void setDemandMap(std::map<std::string, Demand> t_demandMap);
std::map<std::string, Car> getCarMap();
std::map<std::string, User> getUserMap();
std::map<std::string, Demand> getDemandMap();

//pulling a single object as defined by its id from a map
Car pullCar(std::map<std::string, Car> t_carMap, std::string t_id);
User pullUser(std::map<std::string, User> t_userMap, std::string t_id);
Demand pullDemand(std::map<std::string, Demand> t_demandMap, std::string t_id);

//what the scheduler actually does!
int distanceToPickup(Car t_car, int t_pickup);
int compareCarDemand(Car t_car, Demand t_demand);
void scheduleCar(Demand &t_demand);
void closeDemand(Demand &t_demand);
};