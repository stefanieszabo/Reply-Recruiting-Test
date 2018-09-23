#include "rtMgmtCar.h"
#include "rtMgmtUser.h"
#include "rtMgmtDemand.h"
#include "rtScheduler.h"

/*
##########################
RECRUITING TEST
BACKEND ENGINEER INTERNET OF THINGS -- CONCEPT REPLY

Applicant: Stefanie SZABO
Test: Mobility on Demand
Date: 17.09. - 24.09.2018
Task: 2 and 3 (Management/Scheduling Service Implementation)
*/

//This cpp file here is just my personal test playground - it serves to check whether the code works when all values have been set correctly and everything is fluffy.
//##########################

/*Our "DATABASE"!
Since the task allows the use of an 'in-memory' database, I will create a representation of the database tables using maps.
Each object table will be translated into a map that will contain ordered pairs of the object and its object ID. The object ID will be used as a key value,
thus giving me not only a means to access and find an entry with the ID, but it also makes sure that no object lacks an ID. Thanks to the maps having a lot
of existing functions and being pre-sorted, this also saves me time.
A disadvantage of this method is that it's not as easy for me to access a "column" of my "table" in a very quick way as a true database would enable me to do.
However, I can help myself by means of loops and iterators. Nobody said it was easy!*/

//Map representing the car object table
std::map<std::string, Car> oTableCar;

//Map representing the user object table
std::map<std::string, User> oTableUser;

//Map representing the demand object table
std::map<std::string, Demand> oTableDemand;


//##########################

//Let's create some test data. How about some cars?
Car c1(42, "WI-K 4723", "Opel Adam", 3, false); //that's my own car!
Car c2(23, "M-T 357", "Mercedes A-Class", "executive car", engineType::Diesel, 4, lugSpace::M, "leather seats, radio with Bluetooth, cup holder", specServ::Kids, false);
Car c3(65, "HH-OM 1234", "Toyota RAV-4", "SUV", engineType::Petrol, 4, lugSpace::L, "books, big CD collection, driver speaks english", specServ::Pets, false);

//And now three users!
User u1("p1a$s2bla3Word", "lieschenmueller@mail.de", "0211/123456", "Lieschen", 35, gender::female, "", false);
User u2("B1uBM!aU357", "06142/78901", "Philip J. Fry", 25, gender::male, false);
User u3("password123", "0176/333333", "Test User", 75, gender::other, false);

//Create a scheduler for later
Scheduler carSched(oTableCar, oTableUser, oTableDemand);

int main() {
	//Add the objects to the databases
	c1.addCarToDB(oTableCar);
	c2.addCarToDB(oTableCar);
	c3.addCarToDB(oTableCar);
	/*test if that works...
	for (std::map<std::string, Car>::iterator it = oTableCar.begin(); it != oTableCar.end(); it++) {
		it->second.printCar();
	}*/
	u1.addUserToDB(oTableUser);
	u2.addUserToDB(oTableUser);
	u3.addUserToDB(oTableUser);
	//and that works too. Great! :)
	/*for (std::map<std::string, User>::iterator it = oTableUser.begin(); it != oTableUser.end(); it++) {
		it->second.printUser();
	}*/
	u3.removeUser(u3.getUserID(), oTableUser);
	/*for (std::map<std::string, User>::iterator it = oTableUser.begin(); it != oTableUser.end(); it++) {
		it->second.printUser();
	}*/

	//Now that that's settled, let's create a demand and schedule it.
	//Demand testDemand(u1.getUserID(), "", );


	system("Pause");
	return 0;
}
