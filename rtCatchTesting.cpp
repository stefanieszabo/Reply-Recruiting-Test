#define CATCH_CONFIG_RUNNER

#include "rtMgmtCar.h"
#include "rtMgmtUser.h"
#include "rtMgmtDemand.h"
#include "rtScheduler.h"
#include<sstream>
#include<iomanip>
#include "catch.hpp"

/*
##########################
RECRUITING TEST
BACKEND ENGINEER INTERNET OF THINGS -- CONCEPT REPLY

Applicant: Stefanie SZABO
Test: Mobility on Demand
Date: 17.09. - 24.09.2018
Task: 4 (Management/Scheduling Service Testing)
*/

//In order to test if everything works, I implemented tests with the C/C++ test framework Catch
//Source: https://github.com/catchorg/Catch2
//Many thanks to Martin Horenovsky for maintaining this framework!
//##########################

//##########################
/* Oh no, I forgot something - there is no direct conversion from string to time_t, so I will have to
do this the difficult way.
Converts a string with the format hh::mm::ss into a time_t value. I will have to assume that the format is correct.
Many thanks to Stack Overflow for giving me a hand here.
Source: https://stackoverflow.com/questions/11213326/how-to-convert-a-string-variable-containing-time-to-time-t-type-in-c
*/
time_t stringToTime(std::string t_s) {
	struct std::tm timeContainer;
	std::istringstream tempStream(t_s);
	tempStream >> std::get_time(&timeContainer, "%T");
	time_t resTime = mktime(&timeContainer);
	return resTime;

}

/*##########################
/CAR - GENERAL TESTING
	What happens if an attribute is set with a value outside its specific range?
	Are the required attribute values not empty? Does the error handling work if an empty value is added?
	Do the operations "add to DB", "edit Car", and "remove Car" work as intended?
		What happens if I add an already existing car to the DB?
		What if I try to delete or edit a car that does not exist?
##########################*/
SCENARIO("Car attributes are not in the specified ranges", "[Car]") {
	GIVEN("A car with some default settings") {
		Car c1(23, "M-T 357", "Mercedes A-Class", "executive car", engineType::Diesel, 4, lugSpace::M, "leather seats, radio with Bluetooth, cup holder", specServ::Kids, false);

		WHEN("the car location is less than 1") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS( c1.setCarLoc(0) );
			}
		}
		WHEN("the car location changes to a number greater than 1") {
			THEN("this should be okay and cause no problems") {
				REQUIRE_NOTHROW(c1.setCarLoc(5));
			}
		}
		WHEN("the car plate number is empty") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(c1.setCarPlate(""));
			}
		}
		WHEN("the size of the car plate string is greater than 20") {
			THEN("an error needs to be thrown") {
				REQUIRE_THROWS(c1.setCarPlate("more than twenty characters"));
			}
		}
		WHEN("the car model string is empty") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(c1.setCarModel(""));
			}
		}
		WHEN("the size of the car model string is greater than 50") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(c1.setCarModel("this string is definitely longer than fifty characters"));
			}
		}
		WHEN("the size of the car type string is greater than 20") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(c1.setCarType("more than twenty characters"));
			}
		}
		WHEN("we try to specify a number of seats that is less than 1"){
			THEN("an error has to be thrown"){
				REQUIRE_THROWS(c1.setCNumSeats(-1));
			}
		}
		WHEN("we try to specify a number of seats greater than 10") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(c1.setCNumSeats(42));
			}
		}
		WHEN("the size of the comfort options string is greater than 100") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(c1.setCComfort("this string is definitely longer than fifty characters. this string is definitely longer than fifty characters."));
			}
		}
	}
}

SCENARIO("Test the add, edit, and remove functions of the Car class", "[Car]") {
	GIVEN("A car with some default settings and a map that represents the object table") {
		Car c1(23, "M-T 357", "Mercedes A-Class", "executive car", engineType::Diesel, 4, lugSpace::M, "leather seats, radio with Bluetooth, cup holder", specServ::Kids, false);
		std::map<std::string, Car> carMap;

		WHEN("the car is added to the map") {
			THEN("this should not throw an error") {
				REQUIRE_NOTHROW(c1.addCarToDB(carMap));
			}
		}
		WHEN("the car is added to the map") {
			c1.addCarToDB(carMap);
			THEN("the car should be found in the map afterwards") {
				REQUIRE(existsInDB(c1.getCarID(), carMap) == true);
			}
		}
		WHEN("two cars with the same ID are added to the map") {
			c1.addCarToDB(carMap);
			THEN("an error should be thrown") {
				REQUIRE_THROWS(c1.addCarToDB(carMap));
			}
		}
		WHEN("a car is removed from the map") {
			c1.addCarToDB(carMap);
			c1.removeCar(c1.getCarID(), carMap);
			THEN("we should no longer find the ID in the map") {
				REQUIRE(existsInDB(c1.getCarID(), carMap) == false);
			}
		}
		WHEN("we try to remove a car that is not in the database") {
			THEN("we should get an error") {
				REQUIRE_THROWS(c1.removeCar(c1.getCarID(), carMap));
			}
		}
		WHEN("we try to update a car that is not in the database") {
			THEN("we should get an error") {
				REQUIRE_THROWS(c1.updateCar(carMap));
			}
		}
	}
}

/*##########################
/USER - GENERAL TESTING
	What happens if an attribute is set with a value outside its specific range?
	Are the required attribute values not empty? Does the error handling work if an empty value is added?
	Do the operations "add to DB", "edit User", and "remove User" work as intended?
		What happens if I add an already existing user to the DB?
		What if I try to delete or edit a user that does not exist?
##########################
*/
SCENARIO("User attributes are not in the specified ranges", "[User]") {
	GIVEN("A user with some default settings") {
		User u1("p1a$s2bla3Word", "lieschenmueller@mail.de", "0211/123456", "Lieschen", 35, gender::female, "", false);

		WHEN("the user's password is an empty string") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(u1.setUserPWD(""));
			}
		}
		WHEN("the user's password is longer than 25 characters") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(u1.setUserPWD("this string is definitely longer than fifty characters"));
			}
		}
		WHEN("the user's mail address is empty") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(u1.setUMail(""));
			}
		}
		WHEN("the user's mail address is not a mail adress") {
			THEN("an error needs to be thrown") {
				REQUIRE_THROWS(u1.setUMail("Am I truly a mail address?"));
			}
		}
		WHEN("the user's mail address is longer than 50 characters") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(u1.setUMail("this string is definitely longer than fifty characters"));
			}
		}
		WHEN("the user's phone number is empty") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(u1.setUPhone(""));
			}
		}
		WHEN("the size of the user phone number string is greater than 15 characters") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(u1.setUPhone("what if cats wore pants?"));
			}
		}
		WHEN("the user name string is empty") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(u1.setUName(""));
			}
		}
		WHEN("the size of the user phone number string is greater than 20 characters") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(u1.setUName("this string is definitely longer than fifty characters"));
			}
		}
		WHEN("the user's age is less than 18") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(u1.setUAge(-3));
			}
		}
		WHEN("the user's age is more than 117") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(u1.setUAge(1337));
			}
		}
		WHEN("the user's accessibility needs string is longer than 100 characters") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(u1.setUAccess("this string is definitely longer than fifty characters. this string is definitely longer than fifty characters."));
			}
		}
	}
}

SCENARIO("Test the add, edit, and remove functions of the User class", "[User]") {
	GIVEN("A user with some default settings and a map that represents the object table") {
		User u1("p1a$s2bla3Word", "lieschenmueller@mail.de", "0211/123456", "Lieschen", 35, gender::female, "", false);
		std::map<std::string, User> userMap;
		//std::map<std::string, Car>::iterator carIT;

		WHEN("the user is added to the map") {
			THEN("this should not throw an error") {
				REQUIRE_NOTHROW(u1.addUserToDB(userMap));
			}
		}
		WHEN("the car is added to the map") {
			u1.addUserToDB(userMap);
			THEN("the car should be found in the map afterwards") {
				REQUIRE(existsInDB(u1.getUserID(), userMap) == true);
			}
		}
		WHEN("two cars with the same ID are added to the map") {
			u1.addUserToDB(userMap);
			THEN("an error should be thrown") {
				REQUIRE_THROWS(u1.addUserToDB(userMap));
			}
		}
		WHEN("a car is removed from the map") {
			u1.addUserToDB(userMap);
			u1.removeUser(u1.getUserID(), userMap);
			THEN("we should no longer find the ID in the map") {
				REQUIRE(existsInDB(u1.getUserID(), userMap) == false);
			}
		}
		WHEN("we try to remove a car that is not in the database") {
			THEN("we should get an error") {
				REQUIRE_THROWS(u1.removeUser(u1.getUserID(), userMap));
			}
		}
		WHEN("we try to update a car that is not in the database") {
			THEN("we should get an error") {
				REQUIRE_THROWS(u1.updateUser(userMap));
			}
		}
	}
}


/*##########################
/DEMAND - GENERAL TESTING
What happens if an attribute is set with a value outside its specific range?
Are the required attribute values not empty? Does the error handling work if an empty value is added?
	Do the operations "add to DB", "edit Demand", and "remove Demand" work as intended?
	What happens if I add an already existing demand to the DB?
What if I try to delete or edit a demand that does not exist?
Does the error handling work if I try to add non-existant users and cars to the demand?
What happens if the pick-up and drop-off locations are the same? 
What if the drop-off time is earlier than the pick-up time?
What if I assign a car to the demand that is already booked?
What if I try and create a demand for a user that already has an open demand?
What if I try to delete a car/user that is currently assigned to a demand?
##########################
*/




SCENARIO("Demand attributes are not in the specified ranges", "[Demand]") {
	GIVEN("A demand with some default attributes") {
		User u1("p1a$s2bla3Word", "lieschenmueller@mail.de", "0211/123456", "Lieschen", 35, gender::female, "", false);
		std::map<std::string, User> userMap;
		u1.addUserToDB(userMap);
		Car c1(23, "M-T 357", "Mercedes A-Class", "executive car", engineType::Diesel, 4, lugSpace::M, "leather seats, radio with Bluetooth, cup holder", specServ::Kids, false);
		std::map<std::string, Car> carMap;
		c1.addCarToDB(carMap);

		time_t zeitstempel;
		Demand d1(userMap, carMap, u1.getUserID(), "", stringToTime("23:50:00"), 14, stringToTime("23:59:00"), 21, time(&zeitstempel), 2, lugSpace::S, specServ::None, "", dStatus::OPEN);

		WHEN("the user ID of the demand is empty") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(d1.setDUserID(""));
			}
		}
		WHEN("the user ID is not of the specified ID format") {
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(d1.setDUserID("hasi99"));
			}
		}
		WHEN("the user ID is not of a user in the database") {
			User u2("B1uBM!aU357", "06142/78901", "Philip J. Fry", 25, gender::male, false);
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(d1.setDUserID(u2.getUserID()));
			}
		}
		//caught on an error - time's up!
		WHEN("the user with the given ID already has an open demand") {
			u1.setUOrderStatus(true);
			u1.updateUser(userMap);
			THEN("an error has to be thrown") {
				REQUIRE_THROWS(d1.setDUserID(u1.getUserID()));
			}
		}
	}
}

/*##########################
SCHEDULING - GENERAL TESTING
What happens if I try to add maps to the scheduler that don't exist?
What if I try to schedule a car for a demand that does not exist?
What if I try to schedule a car for a demand that is already in progress?
What if I try to schedule a car for a user that does not exist?
What if I try to schedule a car for a user that already has an open demand?
What if I try to schedule a car that does not exist?
What if I try to schedule a car that is already booked?
What happens when I try to calculate the distance car location - pick-up location for a non-existant car?
What happens if I try to find a car's weight for a demand that is not open?
What happens if I try to find a car's weight for a demand that does not exist?
What happens if I try to calculate the weight of a car that does not exist?
When a schedule for a car cannot be found, does the demand status change to nosched?
Do the statuses of users/cars/demands change properly after scheduling?
When closing the demand, do the car/demand/user exist?
Does the car's location correspond to the pick-up location?
Are the car and user statuses updated correctly?
Is the demand deleted properly?
##########################*/

int main(int argc, char ** argv) {

	int result = Catch::Session().run(argc, argv);
	system("pause");
	return result;
}