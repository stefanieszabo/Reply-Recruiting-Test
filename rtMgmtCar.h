#pragma once
#include<iostream>
#include<string>
#include<time.h>
#include<random>
#include<map>
#include <exception>
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
update the current location of the car independently. Fur-thermore, the service should support adding and removing users as well as 
changing the user details. Finally, the service should support adding and removing demands as well as changing demand details.
##########################
*/

//Enums for the car class
//engineType: specifies whether the engine type is petroleum, diesel, hybrid, or electric
enum class engineType : char { Petrol = 'P', Diesel = 'D', Hybrid = 'H', Electric = 'E'};

//lugSpace: specifies the luggage space to be small (S), medium (M) or large (L) - in reality, a sort of volume would make more sense. Also used by demand class!
enum class lugSpace: char {S = 'S', M = 'M', L = 'L'};

//specServe: specified whether the car can transport kids, pets , and/or persons with disabilities
enum class specServ {None, Kids, Pets, Sp_Needs};

//##########################

/*
Car class
Serves as the container for everything that is contained by the object table "CAR" of the database.
Includes the following attributes:
variable name	-	description											-	type in DB	-	type here
carID			-	alphanumeric ID to identify a car					-	varchar(15)	-	string
carLoc			-	car location										-	int			-	int
carPlate		-	car plate as ID for user							-	varchar(20) -	string
carModel		-	manufacturer and model								-	varchar(50) -	string
carType			-	e.g. city car, sportscar etc.						-	varchar(20)	-	string
cEngine			-	petroleum, diesel, hybrid or electric				-	enum		-	enum
cNumSeats		-	number of available seats, must be >= 1				-	tinyint		-	int
cLugSpace		-	size of available luggage space						-	enum		-	enum
cComfort		-	comfort and entertainment options					-	varchar(100)-	string
cSpecialServ	-	 transport of kids, pets or special needs people	-	enum		-	enum
cOrderStatus	-	checks if car is booked (true) or not (false)		-	boolean		-	bool

Also, functions (apart from constructors, sets and gets):
void addCarToDB() - adds a car object as row into CAR table
void editCar() - allows changes to be made to a car row
void removeCar() - removes car row from CAR table
void printCar() - produces an output string on the console for debug purposes

*/

//##########################
class Car {
private:
	//attributes from the CAR table
	std::string carID;
	int carLoc;
	std::string carPlate;
	std::string carModel;
	std::string carType{ "" };
	engineType cEngine{engineType::Petrol};
	unsigned int cNumSeats;
	lugSpace cLugSpace{lugSpace::M};
	std::string cComfort{ "" };
	specServ cSpecialServ{specServ::None};
	bool cOrderStatus{false};
	//generate a quasi-random car ID
	void generateCarID();
public:
	//constructors
	//Car();
	Car(int t_loc, std::string t_plate, std::string t_model, std::string t_type, engineType t_engine, int t_seats, lugSpace t_space, std::string t_comfort, specServ t_sp, bool t_status);
	//minimal car attributes
	Car(int t_loc, std::string t_plate, std::string t_model, int t_seats, bool t_status);

	//set and get functions
	void setCarLoc(int t_loc);
	void setCarPlate(std::string t_plate);
	void setCarModel(std::string t_model);
	void setCarType(std::string t_type);
	void setCEngine(engineType t_engine);
	void setCNumSeats(int t_seats);
	void setCLugSpace(lugSpace t_space);
	void setCComfort(std::string t_comfort);
	void setCSpecialServ(specServ t_sp);
	void setCOrderStatus(bool t_status);

	std::string getCarID();
	int getCarLoc();
	std::string getCarPlate();
	std::string getCarModel();
	std::string getCarType();
	engineType getCEngine();
	int getCNumSeats();
	lugSpace getCLugSpace();
	std::string getCComfort();
	specServ getCSpecialServ();
	bool getCOrderStatus();
	//Holy moly, that was a lot of stuff!
	//other functions as specified

	void updateCar(std::map<std::string, Car> t_map);
	void addCarToDB(std::map<std::string, Car> &t_map);
	void removeCar(std::string t_id, std::map<std::string, Car> &t_map);
	void printCar();
	
};

//Other functions that refer to a car, but are supposed to be implemented independently:
//void updateCarLocation - changes car location according to (hypothetical) incoming data until dropoff location is reached
void updateCarLocation(Car &t_c, int t_dropoff);
//bool existsInDB(std::string t_cID, std::map<std::string, Car> t_map) - returns true if the car with the given ID is in the database
bool existsInDB(std::string t_cID, std::map<std::string, Car> t_map);
