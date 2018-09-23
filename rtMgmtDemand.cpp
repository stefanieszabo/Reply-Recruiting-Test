#include "rtMgmtDemand.h"

/*
##########################
RECRUITING TEST
BACKEND ENGINEER INTERNET OF THINGS -- CONCEPT REPLY

Applicant: Stefanie SZABO
Test: Mobility on Demand
Date: 17.09. - 24.09.2018
Task: 2 (Management Service Implementation)

##########################
Task Description
Design and implement a service for managing the data stored in the database from the previ-ous exercise, i.e. cars, users, and demands.

The service should support adding and removing cars as well as changing the car details. In particular, it should be possible to
update the current location of the car independently. Furthermore, the service should support adding and removing users as well as
changing the user details. Finally, the service should support adding and removing demands as well as changing demand details.
##########################
*/

/*
##########################
DEMAND
##########################
*/

//void generateDemandID() - generates a demand ID
/*To ensure that each demID is unique and can't be changed afterwards, we will just 
"secretly" generate one by adding a randomly generated number to a string.*/
void Demand::generateDemandID() {
	std::string genDemID = "RPLYDMD_";
	//use system time as seed for the random number generator
	unsigned demSeed = time(nullptr);
	std::default_random_engine dGenerator(demSeed);
	std::uniform_int_distribution<int> dDistribution(1, 100000);
	//generate a random number between 1 and 100000
	int demandRand = dDistribution(dGenerator);
	//add random number to the RPLYDMD_ string
	genDemID += demandRand;
	this->demID = genDemID;
}

//Definitions for constructors
//Demand ID is constant and automatically generated for the test purpose here; format: "RPLYDMD_"+random number

Demand::Demand(std::map<std::string, User> t_uMap, std::map<std::string, Car> t_cMap, std::string t_uID, std::string t_cID, time_t t_pTime, int t_pLoc, time_t t_dTime, int t_dLoc, time_t t_timestamp, int t_groupsize, lugSpace t_luggage, specServ t_sp, std::string t_comfort, dStatus t_status){
	this->setUserTable(t_uMap);
	this->setCarTable(t_cMap);
	this->generateDemandID();
	this->setDUserID(t_uID);
	this->dCarID = t_cID;
	this->setDemPTime(t_pTime);
	this->setDemPLoc(t_pLoc);
	this->setDemDTime(t_dTime);
	this->setDemDLoc(t_dLoc);
	this->setDemTimeStamp(t_timestamp);
	this->setDemNumP(t_groupsize);
	this->setDemLuggage(t_luggage);
	this->setDemSpecial(t_sp);
	this->setDemComfort(t_comfort);
	this->setDemStatus(t_status);
}

//minimal demand constructor
Demand::Demand(std::map<std::string, User> t_uMap, std::map<std::string, Car> t_cMap, std::string t_uID, time_t t_pTime, int t_pLoc, time_t t_dTime, int t_dLoc, time_t t_timestamp, int t_groupsize, dStatus t_status){
	this->setUserTable(t_uMap);
	this->setCarTable(t_cMap);
	this->generateDemandID();
	this->setDUserID(t_uID);
	this->setDemPTime(t_pTime);
	this->setDemPLoc(t_pLoc);
	this->setDemDTime(t_dTime);
	this->setDemDLoc(t_dLoc);
	this->setDemTimeStamp(t_timestamp);
	this->setDemNumP(t_groupsize);
	this->setDemStatus(t_status);
}

//demand set functions

void Demand::setUserTable(std::map<std::string, User> t_uMap) {
	this->userTable = t_uMap;
}

void Demand::setCarTable(std::map<std::string, Car> t_cMap) {
	this->carTable = t_cMap;
}

/*void setDUserID(std::string t_uID) - sets the user id of the demand
A demand is placed by a user - so obviously, it is necessary to be able to 
recall who the user is. To this end, I decided to just save the user ID in 
the demand itself. The user ID should not be empty and the user should only 
have one demand open at a time.*/
void Demand::setDUserID(std::string t_uID) {
	std::size_t found = t_uID.find("RPLYUSR_");
	bool userStatus = checkUOrderStatus(t_uID, this->userTable);

	if (t_uID.empty() == true) {
		throw "ERROR: Must contain a user ID!\n";
	}
	else if (found == std::string::npos) {
		throw "ERROR: Not a valid user ID!\n";
	}
	else if (existsInDB(t_uID, userTable) == false) {
		throw "ERROR: User doesn't exist in the database";
	}
	else if (userStatus == true){
		throw "ERROR: User already has an open demand!\n";
	}
	else {
		this->dUserID = t_uID;
	}
}

/*void setDCarID(std::string t_cID) - sets the car id of the demand
A demand is answered by a car that will transport the user to their desired drop-off 
location. It's necessary to document in the demand which car has been scheduled to
respond to it - which is done by this function.
As far as I understand, only the scheduler is supposed to utilize this function. 
He already excludes the possibility that a car can be "overbooked". But one can't be 
safe enough - at least we should check that the ID exists in the DB.*/
void Demand::setDCarID(std::string t_cID) {
	if (existsInDB(t_cID, carTable) == false) {
		throw "ERROR: Car doesn't exist in the database";
	}
	else {
		this->dCarID = t_cID;
	}
	
}
/*void setDemPTime(time_t t_pTime) - sets the earliest pick-up time
We can leave this one simple - conveniently, time_t exists!*/
void Demand::setDemPTime(time_t t_pTime) {
	this->demPTime = t_pTime;
}
/*void setDemPLoc(int t_pLoc) - sets the location code from which to pick up the user
As mentioned in the car file, we encode all locations with an integer. As long as this 
integer is greater than zero and an actual integer, we are fine.*/
void Demand::setDemPLoc(int t_pLoc) {
	if (t_pLoc <= 0) {
		throw "ERROR: Not a valid location code!\n";
	}
	else {
		this->demPLoc = t_pLoc;
	}
	
}
/*setDemPTime(time_t t_pTime) - sets the latest drop-off time.
As long as this time is _after_ the pick-up time, we should be happy. Unfortunately, we 
can't do it like Marty McFly!*/
void Demand::setDemDTime(time_t t_dTime) {
	if (t_dTime < this->demPTime) {
		throw "ERROR: The drop-off time has to be later than the pick-up time!";
	}
	this->demDTime = t_dTime;
}

/*void setDemDLoc(int t_dLoc) - sets the location code at which to drop off the user
As mentioned in the car file, we encode all locations with an integer. As long as this 
integer is greater than zero and not the same as the pick-up location, we are fine.*/
void Demand::setDemDLoc(int t_dLoc) {
	if (t_dLoc <= 0) {
		throw "ERROR: Not a valid location code!\n";
	}
	else if (t_dLoc == this->getDemPLoc()) {
		throw "ERROR: The drop-off location has to differ from the pick-up location!\n";
	}
	else {
		this->demDLoc = t_dLoc;
	}
}
/*void setDemTimeStamp(time_t t_timestamp) - creates a timestamp for the demand
A demand should only have a limited "time to live" - at least an hour or so before the 
user wants to be picked up, he should know that somebody will arrive. If we have a list
of demands, the scheduler should answer the oldest one first. (Ideally, we never have 
a list, of course ;) )*/
void Demand::setDemTimeStamp(time_t t_timestamp) {
	this->demTimeStamp = time(&t_timestamp);
}
/*void setDemNumP(int t_seats) - sets the number of people that require transportation
Most of the time, this will be one person, but sometimes people want to travel with others. 
As long as the number is at least one, we should be fine. To be safe, we will add
a "hard cap" of 10 people - any more and they are probably better off renting a bus.*/
void Demand::setDemNumP(int t_seats) {
	if (t_seats < 1) {
		throw "ERROR: At least one traveler needs to be added! \n";
	}
	else if (t_seats > 10) {
		throw "ERROR: Group limit for our car fleet reached.\n";
	}
	else {
		this->demNumP = t_seats;
	}
}
/*void setDemLuggage(lugSpace t_luggage) - sets the amount of luggage that needs to be 
transported in the categories small (S), medium (M) or large (L). These categories were set in 
place for ease of implementation (as no "real" user would have an idea what those categories 
mean without explanation). For the test, let's assume that "small" means one person's luggage, 
"medium" means the luggage of a four-person group, and "large" means roughly "needs to transport 
half an apartment".
Adding this is optional; the default value is small.
*/
void Demand::setDemLuggage(lugSpace t_luggage) {
	try {
		this->demLuggage = t_luggage;
	}
	catch (std::exception e) {
		std::cout << "ERROR while specifying the amount of luggage: " << e.what() << "\n";
	}
}
/*void setDemSpecial(specServ t_sp) - specifies whether the required car needs to transport kids(1), 
pets(2), or people with disabilities(3). If this is not necessary, one can choose the option "None"(0) 
- this is also the default value. In reality, a user should be able to choose more than one option!
*/
void Demand::setDemSpecial(specServ t_sp) {
	this->demSpecial = t_sp;
}

/*void setDemComfort(std::string t_comfort): specifies the desired comfort and entertainment options of the car.
The length of the string must not exceed 100 characters.
This is an optional information, so an empty string is possible.*/
void Demand::setDemComfort(std::string t_comfort) {
	if (t_comfort.size() > 100) {
		throw "ERROR: Your entry has exceeded the maximum length of 100 characters. Your entry will be resized to 100 characters.\n ";
		t_comfort.resize(100);
	}
	else {
		this->demComfort = t_comfort;
	}

}
/*void setDemStatus(dStatus t_status): sets the current status of the demand: it is either open 
(OPEN, 0), in progress(PROGRESS, 1), closed (CLOSED, 2), or could not be scheduled (NOSCHED, 3).
OPEN means that the demand was placed and the scheduler is looking for a car.
PROGRESS means that a car was assigned to the scheduler and is currently traveling to meet or
drop off the user.
CLOSED means that the user was dropped off and the demand will be deleted shortly.
NOSCHED means that no available car could meet the demand.
The default value is OPEN.*/
void Demand::setDemStatus(dStatus t_status) {
	try {
		this->demStatus = t_status;
	}
	catch (std::exception &e) {
		std::cout << "ERROR while setting the demand status:" << e.what() << "\n";
	}
	
}

//demand get functions
std::string Demand::getDemID(){
	return this->demID;
}
std::string Demand::getDUserID(){
	return this->dUserID;
}
std::string Demand::getDCarID(){
	return this->dCarID;
}
time_t Demand::getDemPTime(){
	return this->demPTime;
}
int Demand::getDemPLoc(){
	return this->demPLoc;
}
time_t Demand::getDemDTime(){
	return this->demDTime;
}
int Demand::getDemDLoc(){
	return this->demDLoc;
}
time_t Demand::getDemTimeStamp(){
	return this->demTimeStamp;
}
int Demand::getDemNumP(){
	return this->demNumP;
}
lugSpace Demand::getDemLuggage(){
	return this->demLuggage;
}
specServ Demand::getDemSpecial(){
	return this->demSpecial;
}
std::string Demand::getDemComfort(){
	return this->demComfort;
}
dStatus Demand::getDemStatus(){
	return this->demStatus;
}

//other functions as specified

/*void addDemandToDB(map<std::string, Demand*> t_map) - adds a demand to the "object table". In our case, the object table
is represented by a map that utilizes the demand ID as a key and the demand itself as the value. This ensures that we
can always refer to a demand by its ID - and that each demand has one.
What we need to check for is that our ID has not been used before.
*/
void Demand::addDemandToDB(std::map<std::string, Demand> &t_map) {
	try {
		t_map.insert(std::pair<std::string, Demand>(this->getDemID(), *this));
	}
	catch (std::exception &e) {
		std::cout << "ERROR while adding element to DB: " << e.what() << "\n";
	}
}
/*void updateDemand(std::map<std::string, Demand*> t_map) - updates the demand saved in the map after changes have been made
I am not really sure if this works. For simplicity reasons, I decided to just brute-force it by "overriding" the demand 
in the map with the one that was changed. There are probably more elegant solutions, I am sorry.*/
void Demand::updateDemand(std::map<std::string, Demand> &t_map) {
	std::string idToUpdate = this->getDemID();
	std::map<std::string, Demand>::iterator idFinder;
	idFinder = t_map.find(idToUpdate);
	try {
		idFinder->second = *this;
	}
	catch (std::exception e) {
		std::cout << "ERROR while updating element: " << e.what() << "\n";
	}
}

/*void removeDemand(std::string t_id, map<std::string, Demand*> t_map) - removes a user with a given ID. By utilizing
the map function find, we first search whether a pair with the ID still exists in the map. We try and erase it and
catch an error when the ID can no longer be found.
*/
void Demand::removeDemand(std::string t_id, std::map<std::string, Demand> &t_map) {
	if (this->getDemStatus() == dStatus::PROGRESS) {
		throw "ERROR: This demand is in progress and cannot be deleted! \n";
	}
	else {
		std::map<std::string, Demand>::iterator idFinder;
		idFinder = t_map.find(t_id);
		try {
			t_map.erase(idFinder);
		}
		catch (std::invalid_argument &e) {
			std::cout << "ERROR: ID is not in the database! " << e.what() << "\n";
		}
	}
}
/*void printDemand(): this function just prints out the demand data on the console to check it*/
void Demand::printDemand() {
	//create a legible printout for the demand status and assigned car ID
	std::string demandStatus, assignedCar = "";

	switch (this->getDemStatus()) {
	case dStatus::PROGRESS:
		demandStatus = "PROGRESS";
		assignedCar = this->getDCarID();
		break;
	case dStatus::CLOSED:
		demandStatus = "CLOSED";
		assignedCar = this->getDCarID();
		break;
	case dStatus::NOSCHED:
		demandStatus = "NO SCHEDULE FOUND";
		assignedCar = "None";
		break;
	default:
		demandStatus = "OPEN";
		assignedCar = "Looking for it!";
	}
	//and blatantly steal the printouts for luggage and special service from the car class
	//printout for luggage space:
	std::string lug = "";
	switch (this->getDemLuggage()) {
	case lugSpace::S:
		lug = "Small";
		break;
	case lugSpace::L:
		lug = "Large";
		break;
	default:
		lug = "Medium";
	}

	//printout for special services
	std::string spServ = "";
	switch (this->getDemSpecial()) {
	case specServ::Kids:
		spServ = "Children";
		break;
	case specServ::Pets:
		spServ = "Pets";
		break;
	case specServ::Sp_Needs:
		spServ = "Persons with disabilities";
		break;
	default:
		spServ = "None";
	}

	//make times readable by humans - unfortunately, the time format in C++ is... well, what it is.
	//timestamp of the demand
	time_t tempTime = this->getDemTimeStamp();
	struct tm* dTime = nullptr;
	localtime_s(dTime, &tempTime);
	char timestampBuf[50];
	strftime(timestampBuf, 50, "%a %Y.%m.%d, %H:%M", dTime);
	std::string finalTimestamp = timestampBuf;
	//pick-up time
	tempTime = this->getDemPTime();
	localtime_s(dTime, &tempTime);
	strftime(timestampBuf, 50, "%H:%M", dTime);
	std::string pickupTime = timestampBuf;
	//drop-off time
	tempTime = this->getDemDTime();
	localtime_s(dTime, &tempTime);
	strftime(timestampBuf, 50, "%H:%M", dTime);
	std::string dropoffTime = timestampBuf;

	std::cout << "DEMAND " << "created at" << timestampBuf << this->getDemID() << "by user " << this->getDUserID() << ", status " << demandStatus << "\n" <<
		"assigned car: " << assignedCar << "\n" <<
		"earliest pick-up time: " << pickupTime << " at location code: " << this->getDemPLoc() << "; latest drop-off time: " << dropoffTime << " at location code: " << this->getDemDLoc() << "\n" <<
		"group size: " << this->getDemNumP() << ", amount of luggage: " << lug << ", special service options: " << spServ << "\n" <<
		"comfort options: " << this->getDemComfort() << "\n";
}

/*bool existsInDB(std::map<std::string, Demand> t_map): this function checks whether this demand is in the database*/
bool existsInDB(std::string t_dID, std::map<std::string, Demand> t_map) {
	bool demIsInDB = false;
	std::map<std::string, Demand>::iterator mapIT = t_map.begin();
	mapIT = t_map.find(t_dID);
	if (mapIT != t_map.end()) {
		demIsInDB = true;
	}
	return demIsInDB;
}