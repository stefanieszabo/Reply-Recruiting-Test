#include "rtMgmtCar.h"

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
CAR
##########################
*/

//void generateCID() - generates a car ID
/*To ensure that each carID is unique and can't be changed afterwards, we will just "secretly" generate one by adding a randomly generated number to a string.*/
unsigned carSeed = time(nullptr);
std::default_random_engine rtCGenerator(carSeed);
std::uniform_int_distribution<int> rtCDistribution(1, 100000);

void Car::generateCarID() {
	std::string genCarID = "RPLYCAR_";
	rtCDistribution.reset();
	//generate a random number between 1 and 100000
	int carRand = rtCDistribution(rtCGenerator);
	//add random number to the RPLYCAR_ string
	genCarID += std::to_string(carRand);
	this->carID = genCarID;
	
}


//Definitions for constructors and destructor
Car::Car(int t_loc, std::string t_plate, std::string t_model, std::string t_type, engineType t_engine, int t_seats, lugSpace t_space, std::string t_comfort, specServ t_sp, bool t_status) {
	this->generateCarID();
	this->setCarLoc(t_loc);
	this->setCarPlate(t_plate);
	this->setCarModel(t_model);
	this->setCarType(t_type);
	this->setCEngine(t_engine);
	this->setCNumSeats(t_seats);
	this->setCLugSpace(t_space);
	this->setCComfort(t_comfort);
	this->setCSpecialServ(t_sp);
	this->setCOrderStatus(t_status);
}

//minimal car constructor
Car::Car(int t_loc, std::string t_plate, std::string t_model, int t_seats, bool t_status) {
	this->generateCarID();
	this->setCarLoc(t_loc);
	this->setCarPlate(t_plate);
	this->setCarModel(t_model);
	this->setCNumSeats(t_seats);
	this->setCOrderStatus(t_status);
}

//car set functions

/*void setCarLoc(int t_loc): sets a car location. To make things easier, we "encode" all possible locations with a number. 
A location has to be given to determine where the car actually is (and to determine distance from the user etc.)
In a real-world example, it would be most sensible to use GPS coordinates as location. */
void Car::setCarLoc(int t_loc) {
	if (t_loc <= 0) {
		throw "ERROR: Not a valid location code!\n";
	}
	else {
		this->carLoc = t_loc;	
	}
}

/*void setCarPlate(std::string t_plate): sets a plate number. The length of the string must not 
exceed 20 characters and a plate number has to be given (a user should have a way to 
definitely recognize their ride!)
This should be enough for most European plates.*/
void Car::setCarPlate(std::string t_plate) {
	if (t_plate.empty() == true) {
		throw "ERROR: A plate number is required!\n";
	}
	else if (t_plate.size() > 20) {
		throw "ERROR: Your entry has exceeded the maximum length of 20 characters. Your entry will be resized to 20 characters.\n";
		t_plate.resize(20);
	}
	else {
		this->carPlate = t_plate;
	}
}
/*void setCarModel(std::string t_model): sets a model. This consists of a manufacturer and model name.
The length of the string must not exceed 50 characters.
A model has to be given (a user should have a way to definitely recognize their ride!)*/
void Car::setCarModel(std::string t_model) {
	if (t_model.empty() == true) {
		throw "ERROR: This string is empty! Please enter a manufacturer and model name.\n";
	}
	else if (t_model.size() > 50) {
		throw "ERROR: Your entry has exceeded the maximum length of 50 characters. \n";
	}
	else {
		this->carModel = t_model;
	}
}

/*void setCarType(std::string t_type): sets a car type, such as sports car or SUV. The length of the string
must not exceed 20 characters. This information is optional, so an empty string is OK.*/
void Car::setCarType(std::string t_type) {
	if (t_type.size() > 20) {
		throw "ERROR: Your entry has exceeded the maximum length of 20 characters. It will be resized to 20 characters. \n";
		t_type.resize(20);
	}
	else {
		this->carType = t_type;
	}
}

/*void setCEngine(engineType t_engine): sets the type of engine - to be precise, what kind of fuel the engine uses.
Options are Petrol (P), Diesel (D), Hybrid (H), and Electric (E) (which should be explanatory). This information is optional; if none
is given, the default value of "Petrol" is used.
In reality, it would probably make sense to either give an "empty" option, making this option mandatory or leave it
out entirely.*/
void Car::setCEngine(engineType t_engine) {
	try {
		this->cEngine = t_engine;
	}
	catch (std::exception &e) {
		std::cout << e.what() << "\n";
	}
}

/*void Car::setCNumSeats(int t_seats): sets the amount of available passenger seats. At least one seat must be available.
Of course, this generally depends on the kind of car. However, to avoid ridiculous numbers, a "hard cap" of 10 seats is in place.
If there are more than 10 seats or less than 1 seat specified, default values of 10 resp. 1 are assumed.*/
void Car::setCNumSeats(int t_seats) {
	if (t_seats < 1) {
		throw "ERROR! A car must have at least one available passenger seat. A default value of 1 is assumed.\n";
		this->cNumSeats = 1;
	}
	else if (t_seats > 10) {
		throw "WARNING: The amount of seats is limited to 10.\n";
		this->cNumSeats = 10;
	}
	else {
		this->cNumSeats = t_seats;
	}
}

/*void Car::setCLugSpace(lugSpace t_space): specifies the available luggage space in three possible categories: small (S), 
medium (M), and large (L).
These categories were set in place for ease of implementation (as no "real" user would have an idea what those categories mean without
explanation). For the test, let's assume that "small" means room for one person's luggage, "medium" means room for the luggage of a
four-person group, and "large" means roughly "could fit a rhino if we squeeze a little".
As default value, we assume that a car has medium luggage space.*/
void Car::setCLugSpace(lugSpace t_space) {
	try {
		this->cLugSpace = t_space;
	}
	catch (std::exception &e) {
		std::cout << e.what() << "\n";
	}
}

/*void Car::setCComfort(std::string t_comfort): specifies the available comfort and entertainment options of the car.
The length of the string must not exceed 100 characters.
This is an optional information, so an empty string is possible.*/ 
void Car::setCComfort(std::string t_comfort) {
	if (t_comfort.size() > 100) {
		throw "ERROR: Your entry has exceeded the maximum length of 100 characters. Your entry will be resized to 100 characters.\n ";
		t_comfort.resize(100);
	}
	else {
		this->cComfort = t_comfort;
	}
}

/*void Car::setCSpecialServ(specServ t_sp): specifies whether the car is able to transport special passengers.
If this is not possible, the option "None" (0) may be chosen. If it is possible, one can choose "Kids" (1), "Pets" (2)"
or "Special Needs Persons" (3). The default value is "none".
In reality, a car should be able to choose more than one option!*/
void Car::setCSpecialServ(specServ t_sp) {
	try {
		this->cSpecialServ = t_sp;
	}
	catch (std::exception &e) {
		std::cout << e.what() << "\n";
	}
	
}
/*void Car::setCOrderStatus(bool t_status): specifies whether the car is open for demands (false) or booked (true).
The default value is false.*/
void Car::setCOrderStatus(bool t_status) {
	this->cOrderStatus = t_status;
}

//car get functions

std::string Car::getCarID() {
	return this->carID;
}
int Car::getCarLoc() {
	return this->carLoc;
}

std::string Car::getCarPlate() {
	return this->carPlate;
}
std::string Car::getCarModel() {
	return this->carModel;
}
std::string Car::getCarType() {
	return this->carType;
}
engineType Car::getCEngine() {
	return this->cEngine;
}
int Car::getCNumSeats() {
	return this->cNumSeats;
}
lugSpace Car::getCLugSpace() {
	return this->cLugSpace;
}
std::string Car::getCComfort() {
	return this->cComfort;
}
specServ Car::getCSpecialServ() {
	return this->cSpecialServ;
}
bool Car::getCOrderStatus() {
	return this->cOrderStatus;
}

//other functions as specified
/*void addCarToDB(map<std::string, Car> t_map) - adds a car to the "object table". In our case, the object table
is represented by a map that utilizes the car ID as a key and the car itself as the value. This ensures that we
can always refer to a car by its ID - and that each car has one.
What we need to check for is that our ID has not been used before.
*/
void Car::addCarToDB(std::map<std::string, Car> &t_map) {
	std::map<std::string, Car>::iterator t_mapIT = t_map.find(this->getCarID());
	if (t_mapIT != t_map.end()) {
		throw "ERROR: Car with this ID is already in the map!\n";
	}
	else {
		try {
			t_map.insert(std::pair<std::string, Car>(this->getCarID(), *this));
		}
		catch (std::exception &e) {
			std::cout << "ERROR while adding element to DB " << e.what() << "\n";
		}
	}
}

/*void updateCar(std::map<std::string, Car*> t_map) - updates the car saved in the map after changes have been made
I am not really sure if this works. For simplicity reasons, I decided to just brute-force it by "overriding" the car in the map with the one that was changed.
There are probably more elegant solutions, I am sorry.*/
void Car::updateCar(std::map<std::string, Car> t_map) {
	std::string idToUpdate = this->getCarID();
	std::map<std::string, Car>::iterator idFinder;
	idFinder = t_map.find(idToUpdate);
	if (idFinder == t_map.end()) {
		throw "ERROR: ID is not in the database!\n";
	}
	else {
		try {
			idFinder->second = *this;
		}
		catch (std::exception e) {
			std::cout << "ERROR while updating element: " << e.what() << "\n";
		}
	}
}
/*void removeCar(std::string t_id, map<std::string, Car*> t_map) - removes a car with a given ID. By utilizing
the map function find, we first search whether a pair with the ID still exists in the map. We try and erase it and
catch an error when the ID can no longer be found.
*/
void Car::removeCar(std::string t_id, std::map<std::string, Car> &t_map) {
	if (this->getCOrderStatus() == true) {
		throw "ERROR: Car is currently booked and cannot be deleted!\n";
	}
	else {
		std::map<std::string, Car>::iterator idFinder;
		try {
			idFinder = t_map.find(t_id);
			if (idFinder != t_map.end()) {
				t_map.erase(idFinder);
			}
			else {
				throw "ERROR: ID is not in the database!\n";
			}
		}
		catch (std::exception &e) {
			std::cout << "Error while deleting the car from the database: " << e.what() << "\n";
		}
	}
}

/*void Car::printCar(): this function just prints out the car on the console to check it.*/
void Car::printCar() {

	//creating a legible printout for engineTypes:
	std::string engine = "";
	switch (this->getCEngine()) {
	case engineType::Diesel:
		engine = "Diesel";
		break;
	case engineType::Hybrid:
		engine = "Hybrid";
		break;
	case engineType::Electric:
		engine = "Electric";
		break;
	default:
		engine = "Petrol";

	}

	//also, a printout for luggage space:
	std::string lug = "";
	switch (this->getCLugSpace()) {
	case lugSpace::S:
		lug = "Small";
		break;
	case lugSpace::L:
		lug = "Large";
		break;
	default:
		lug = "Medium";
	}

	//a printout for special services
	std::string spServ = "";
	switch (this->getCSpecialServ()) {
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

	//and finally, a printout for the order status!
	std::string orderStat = "";
	if (this->getCOrderStatus() == true) {
		orderStat = "Booked";
	}
	else {
		orderStat = "Available";
	}

	std::cout << "CAR " << this->getCarID() << ", Model: " << this->getCarModel() << "\n" <<
		"Plate number: " << this->getCarPlate() << ", type: " << this->getCarType() <<
		", engine type " << engine << " at location code " << this->getCarLoc() <<
		"\n number of available seats: " << this->getCNumSeats() <<
		"\n luggage space: " << lug <<
		"\n special transport options: " << spServ <<
		"\n Comfort options: " << this->getCComfort() <<
		"\n Order status: " << orderStat << "\n";
}

/*void updateCarLocation(): this function updates the car location.
This function simulates car movement. It increments the car's position until the value of the destination is reached.
*/
void updateCarLocation(Car &t_c, int t_destination) {
	int firstLoc = t_c.getCarLoc();
	int currentLoc = firstLoc;
	while (currentLoc < t_destination) {
		//sleep option to simulate some passing of time, e.g. wait for 10 seconds before next incrementation
		currentLoc++;
		t_c.setCarLoc(currentLoc);
	}
}

/*existsInDB(std::string t_cID, std::map<std::string, Car> t_map): this function checks whether this car is in the database*/
bool existsInDB(std::string t_cID, std::map<std::string, Car> t_map) {
	bool carIsInDB = false;
	std::map<std::string, Car>::iterator mapIT = t_map.begin();
	mapIT = t_map.find(t_cID);
	if (mapIT != t_map.end()) {
		carIsInDB = true;
	}
	return carIsInDB;
}