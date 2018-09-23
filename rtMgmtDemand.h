#pragma once
#include "rtMgmtCar.h"
#include "rtMgmtUser.h"

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

//enums for the demand class
//demStatus - specifies the demand status 
//open: demand was not yet answered by car, progress: demand is answered, closed: user has been dropped off, nosched: no car was able to answer the schedule
enum class dStatus{OPEN, PROGRESS, CLOSED, NOSCHED};

//##########################

/*
Demand class
Serves as the container for everything that is contained by the object table "DEMAND" of the database.

Includes the following attributes:
variable name	-	description														-	type in DB	-	type here
demID			-	alphanumeric ID to identify a demand							-	varchar(15)	-	string
userID			-	(foreign key) ID of user that placed the demand					-	varchar(15)	-	string
carID			-	(foreign key) ID of car that answers the demand (can be NULL!)	-	varchar(15)	-	string
demPTime		-	earliest pick-up time											-	time		-	
demPLoc			-	pick-up location (int for simplicity)							-	int			-	int
demDTime		-	latest drop-off time											-	time		-
demDLoc			-	drop-off location (int for simplicity)							-	int			-	int
demTimeStamp	-	time when demand was placed										-	datetime	-
demNumP			-	number of passengers to transport, must at least be 1			-	tinyint		-	int
demLuggage		-	how much luggage needs to be transported						-	enum		-	enum
demSpecial		-	kids, pets or disabled people that are transported (can be NULL)-	enum		-	enum
demComfort		-	desired comfort options											-	varchar(100)-	string
demStatus		-	checks if demand can be/is fulfilled							-	enum		-	enum

Also, functions (apart from constructors, sets and gets):
void addDemandToDB(User u) - adds a demand object as row into DEMAND table and updates USER table
void editDemand() - allows changes to be made to a demand row
void removeDemand() - removes demand row from DEMAND table and updates USER and CAR tables accordingly
void printDemand() -  prints demand data to console for test purposes
*/
//##########################

class Demand {
private:
	std::string demID;
	std::string dUserID;
	std::string dCarID{""};
	time_t demPTime;
	int demPLoc;
	time_t demDTime;
	int demDLoc;
	time_t demTimeStamp;
	int demNumP;
	lugSpace demLuggage{lugSpace::S};
	specServ demSpecial{specServ::None};
	std::string demComfort{""};
	dStatus demStatus{dStatus::OPEN};

	//generates a demand ID
	void generateDemandID();

	//needed to check user order status and existance
	std::map<std::string, User> userTable;
	//needed to check car order status and existance
	std::map<std::string, Car> carTable;

public:
	//constructors and destructor
	//Demand();
	Demand(std::map<std::string, User> t_uMap, std::map<std::string, Car> t_cMap, std::string t_uID, std::string t_cID, time_t t_pTime, int t_pLoc, time_t t_dTime, int t_dLoc, time_t t_timestamp, int t_groupsize, lugSpace t_luggage, specServ t_sp, std::string t_comfort, dStatus t_status);
	//minimal demand attributes
	Demand(std::map<std::string, User> t_uMap, std::map<std::string, Car> t_cMap, std::string t_uID, time_t t_pTime, int t_pLoc, time_t t_dTime, int t_dLoc, time_t t_timestamp, int t_groupsize, dStatus t_status);

	//set and get functions
	void setDUserID(std::string t_uID);
	void setDCarID(std::string t_cID);
	void setDemPTime(time_t t_pTime);
	void setDemPLoc(int t_pLoc);
	void setDemDTime(time_t t_dTime);
	void setDemDLoc(int t_dLoc);
	void setDemTimeStamp(time_t t_timestamp);
	void setDemNumP(int t_seats);
	void setDemLuggage(lugSpace t_luggage);
	void setDemSpecial(specServ t_sp);
	void setDemComfort(std::string t_comfort);
	void setDemStatus(dStatus t_status);
	void setUserTable(std::map<std::string, User> t_uMap);
	void setCarTable(std::map<std::string, Car> t_cMap);

	std::string getDemID();
	std::string getDUserID();
	std::string getDCarID();
	time_t getDemPTime();
	int getDemPLoc();
	time_t getDemDTime();
	int getDemDLoc();
	time_t getDemTimeStamp();
	int getDemNumP();
	lugSpace getDemLuggage();
	specServ getDemSpecial();
	std::string getDemComfort();
	dStatus getDemStatus();

	//other functions as specified
	void addDemandToDB(std::map<std::string, Demand> &t_map);
	void updateDemand(std::map<std::string, Demand> &t_map);
	void removeDemand(std::string t_id, std::map<std::string, Demand> &t_map);
	void printDemand();
	;
};

//bool existsInDB(std::string t_dID, std::map<std::string, Demand> t_map) - returns true if the demand is in the database
bool existsInDB(std::string t_dID, std::map<std::string, Demand> t_map);