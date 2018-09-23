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
update the current location of the car independently. Furthermore, the service should support adding and removing users as well as
changing the user details. Finally, the service should support adding and removing demands as well as changing demand details.
##########################
*/

//Enum for the user class
//gender: Specifies the user's gender
enum class gender {male, female, other};

//##########################

/*
User class
Serves as the container for everything that is contained by the object table "USER" of the database.
For time/simplicity reasons, secure hashing and encrypting sensitive data is omitted here - I'm aware that
one should never do that in real applications!

Includes the following attributes:
variable name	-	description													-	type in DB	-	type here
userID			-	alphanumeric ID to identify a user							-	varchar(15)	-	string
userPWD			-	password for user account, stored as 'hash'					-	varchar(25)	-	string
uMail			-	user's mail address											-	varchar(50)	-	string
uPhone			-	user's phone number											-	varchar(15)	-	string
uName			-	user's nickname												-	varchar(20)	-	string
uAge			-	user's age, must be over 18									-	tinyint		-	int
uGender			-	male, female or other										-	enum		-	enum
uAccess			-	user's accessibility needs, e.g. assistance to enter car	-	varchar(100)-	string
uOrderStatus	-	checks if the user has an open request (true) or not (false)-	boolean		-	bool

Also, functions (apart from constructors, sets and gets):
void addUserToDB() - adds a user object as row into USER table
void editUser() - allows changes to be made to a user row
void removeUser() - removes user row from USER table
void printUser() - prints user data to console for test purposes


Also added to place a demand:
bool checkUOrderStatus(std::string t_userID, std::map<std::string, User> t_uTable) - checks if the user does not have an open demand
*/

//##########################



class User {
private:
	//attributes from the USER table
	std::string userID;
	std::string userPWD;
	std::string uMail{"test@mail.de"};
	std::string uPhone;
	std::string uName;
	int uAge;
	gender uGender{gender::other};
	std::string uAccess{""};
	bool uOrderStatus{false};
	//generate a quasi-random user ID
	void generateUID();

public:
	//constructors
	//User();
	User(std::string t_pwd, std::string t_mail, std::string t_phone, std::string t_name, int t_age, gender t_gender, std::string t_access, bool t_status);
	//minimal user attributes
	User(std::string t_pwd, std::string t_phone, std::string t_name, int t_age, gender t_gender, bool t_status);

	//set and get functions
	void setUserPWD(std::string t_pwd);
	void setUMail(std::string t_mail);
	void setUPhone(std::string t_phone);
	void setUName(std::string t_name);
	void setUAge(int t_age);
	void setUGender(gender t_gender);
	void setUAccess(std::string t_access);
	void setUOrderStatus(bool t_status);

	std::string getUserID();
	std::string getUserPWD();
	std::string getUMail();
	std::string getUPhone();
	std::string getUName();
	int getUAge();
	gender getUGender();
	std::string getUAccess();
	bool getUOrderStatus();

	//other functions as specified
	void addUserToDB(std::map<std::string, User> &t_map);
	void updateUser(std::map<std::string, User> &t_map);
	void removeUser(std::string t_id, std::map<std::string, User> &t_map);
	void printUser();
	
};
//bool existsInDB(std::map<std::string, User> t_map) - returns true if the user with the given ID is in the database
bool existsInDB(std::string t_uID, std::map<std::string, User> t_map);

//function needed to place a demand
bool checkUOrderStatus(std::string t_userID, std::map<std::string, User> t_uTable);