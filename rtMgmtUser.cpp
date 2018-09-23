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
update the current location of the car independently. Fur-thermore, the service should support adding and removing users as well as
changing the user details. Finally, the service should support adding and removing demands as well as changing demand details.
##########################
*/

/*
##########################
USER
##########################
*/

//void generateUID() - generates a user ID
/*To ensure that each userID is unique and can't be changed afterwards, we will just "secretly" generate one by adding a randomly generated number to a string.*/
unsigned usrSeed = time(nullptr);
std::default_random_engine uGenerator(usrSeed);
std::uniform_int_distribution<int> uDistribution(1, 100000);
void User::generateUID() {
	std::string genUserID = "RPLYUSR_";
	//generate a random number between 1 and 100000
	int usrRand = uDistribution(uGenerator);
	//add random number to the RPLYUSR_ string
	genUserID += std::to_string(usrRand);
	this->userID = genUserID;
}


//Definitions for constructors
//User ID is constant and automatically generated for the test purpose here; format: "RPLYUSR_"+random number

User::User(std::string t_pwd, std::string t_mail, std::string t_phone, std::string t_name, int t_age, gender t_gender, std::string t_access, bool t_status){
	this->generateUID();
	this->setUserPWD(t_pwd);
	this->setUMail(t_mail);
	this->setUPhone(t_phone);
	this->setUName(t_name);
	this->setUAge(t_age);
	this->setUGender(t_gender);
	this->setUAccess(t_access);
	this->setUOrderStatus(t_status);
}

//minimal user constructor
User::User(std::string t_pwd, std::string t_phone, std::string t_name, int t_age, gender t_gender, bool t_status){
	this->generateUID();
	this->setUserPWD(t_pwd);
	this->setUPhone(t_phone);
	this->setUName(t_name);
	this->setUAge(t_age);
	this->setUGender(t_gender);
	this->setUOrderStatus(t_status);
}

//user set functions

/*void setUserPWD(std::string t_pwd): sets a user password
Obviously, for security reasons one should never store passwords in plaintext, as that's how you get data leaks. ;)
To mimic hashing and encryption, we at least create the hash of the given password string and store that in the database.*/
void User::setUserPWD(std::string t_pwd){
	if (t_pwd.empty() == true) {
		throw "ERROR: You need to set a password!\n";
	}
	else if (t_pwd.size() > 25) {
		throw "ERROR: maximal password length is 25 characters!\n"; //I know - no security person would ever throw that error! 
		//And storing passwords in a database is not a good idea either, I guess...
	}
	else {
		std::hash<std::string> pwdHash;
		size_t scrambledEggs = pwdHash(t_pwd);
		this->userPWD = std::to_string(scrambledEggs);
	}
}

/*void setUMail(std::string t_mail): sets a user's email address. 
In some way, a user needs to be contacted, e.g. when their car order could be scheduled (or not), 
if their car is on the way or in other situations. Since the test doesn't exactly say whether our service 
is web- or mobile-based, I will assume both and require both a phone number and a mail address. In reality, 
it would probably be a good idea to check whether the mail domain exists and validate the email address. 
These operations are omitted here.
A mail address cannot be empty and the string may at most be 50 characters long.*/
void User::setUMail(std::string t_mail){
	std::size_t found = t_mail.find('@');
	if(t_mail.empty() == true){ 
		throw "ERROR: A valid email address needs to be included.\n";
	}else if(t_mail.size() > 50) {
		throw "ERROR: Your entry has exceeded the maximum length of 50 characters. \n";
	}
	else if(found == std::string::npos){
		throw "WARNING: This does not seem to be a valid mail address. Please check the entry.\n";
	}
	else {
		this->uMail = t_mail;
	}
}

/*void setUPhone(std::string t_phone): sets the phone number of the user. In some way, a user needs to be contacted, e.g. when their car order could be scheduled (or not), if their car is on the way or in other situations. Since the test doesn't exactly say whether our service is web- or mobile-based, I will assume both and require both a phone number and a mail address.
In reality, it would probably be a good idea to validate the phone number in some way and ensure that the country prefix is valid. These operations are omitted here for simplicity.
A phone number may at most be 15 characters long.*/
void User::setUPhone(std::string t_phone){
	if(t_phone.empty() == true){ 
		throw "ERROR: A valid phone number needs to be included.\n";
	}else if(t_phone.size() > 15) {
		throw "ERROR: Your entry has exceeded the maximum length of 15 characters. It will be resized to 15 characters.\n";
		t_phone.resize(15);
	}
	else {
		this->uPhone = t_phone;
	}
}

/*void setUName(std::string t_name): sets the user's name or nickname. This is more or less to help users and car drivers to find and recognize each other. 
The value will only be checked for length. In reality, it is probably appropriate to use a more sophisticated filter in order to avoid usernames being something offensive.*/
void User::setUName(std::string t_name) {
	if (t_name.empty() == true) {
		throw "ERROR: Please give us a name so your driver can identify you!\n";
	}
	else if (t_name.size() > 20) {
		throw "ERROR: Your entry has exceeded the maximum length of 20 characters. It will be resized to 20 characters\n";
		t_name.resize(20);
	}
	else {
		this->uName = t_name;
	}
}

/*void setUAge(int t_age): sets the user's age. To use the app, the users have to be at least 18 years old. This constraint was chosen for the test because of legal personality/capacity reasons, but is more or less arbitrary. A lower restriction might make more sense, but I am not sure.
To ensure that no joke answers can be set, we will insert a hard cap of 117 (as far as I know, the oldest human of all time lived to be 117, so that's our limit).*/
void User::setUAge(int t_age){
	if(t_age < 18){
	throw "ERROR: The minimum age to utilize this service is 18.\n";
	}else if(t_age > 117){
	throw "WARNING: The known maximum age of humans is 117. The default highest value of 117 will be assumed.\n";
	this->uAge = 117;
	}else{
	this->uAge = t_age;
	}
	
}

/*void setUGender(gender t_gender): sets the user's gender. Possible options are male, female, or other. The default option is other */
void User::setUGender(gender t_gender){
	try {
		this->uGender = t_gender;
	}
	catch (std::exception &e) {
		std::cout << e.what() << "\n";
	}
	
}
/*void setUAccess(std::string t_access): sets the user's accessibility options as specified.
Some people might have physical disabilities, whether temporary or permanent. They may require assistance to enter the car, have a leg cast and/or crutches, or even use a wheelchair. Therefore, they have some special requirements to use a mobility on demand service - e.g., more leg room, enough room to store the wheelchair etc. An enum with some options is probably the better option - I used a string here to keep it simple.
This is an optional field, so an empty string is possible. It's length, however, is constrained to 100 characters. The default value is an empty field.*/
void User::setUAccess(std::string t_access){
	if(t_access.length() > 100){
		throw "ERROR: The entered string is too long. Please try to keep this shorter.";
		t_access.resize(100);
	}
	this->uAccess = t_access;
}

/*void setOrderStatus(bool t_status): checks whether an user currently has an open request (true) or not (false).
The default value is false.*/
void User::setUOrderStatus(bool t_status){
	try {
		this->uOrderStatus = t_status;
	}
	catch (std::exception &e) {
		std::cout << e.what() << "\n";
	}

}


//user get functions
std::string User::getUserID(){
	return this->userID;
}
std::string User::getUserPWD(){
	return this->userPWD;
}
std::string User::getUMail(){
	return this->uMail;
}
std::string User::getUPhone(){
	return this->uPhone;
}
std::string User::getUName() {
	return this->uName;
}
int User::getUAge(){
	return this->uAge;
}
gender User::getUGender(){
	return this->uGender;
	}
std::string User::getUAccess(){
	return this->uAccess;
}
bool User::getUOrderStatus(){
	return this->uOrderStatus;
}

//other functions as specified

/*void addUserToDB(map<std::string, User*> t_map) - adds a user to the "object table". In our case, the object table
is represented by a map that utilizes the user ID as a key and the user itself as the value. This ensures that we
can always refer to a user by its ID - and that each user has one.
What we need to check for is that our ID has not been used before.
*/
void User::addUserToDB(std::map<std::string, User> &t_map){
	if (t_map.find(this->getUserID()) != t_map.end()){
		throw "ERROR: User with this ID is already in the database!\n";
	}
	else {
		try {
			t_map.insert(std::pair<std::string, User>(this->getUserID(), *this));
		}
		catch (std::exception &e) {
			std::cout << "ERROR while adding element to DB" << e.what() << "\n";
		}
	}
}

/*
void updateUser() - updates the user saved in the map after changes have been made
I am not really sure if this works. For simplicity reasons, I decided to just brute-force it by "overriding" the user in the map with the one that was changed.
There are probably more elegant solutions, I am sorry.
*/
void User::updateUser(std::map<std::string, User> &t_map){
	std::string idToUpdate = this->getUserID();
	std::map<std::string, User>::iterator idFinder;
	idFinder = t_map.find(idToUpdate);
	if (idFinder == t_map.end()) {
		throw "ERROR: ID is not in the database!\n";
	}
	else {
		try {
			t_map.erase(idToUpdate);
			this->addUserToDB(t_map);
		}
		catch (std::exception e) {
			std::cout << "ERROR while updating element: " << e.what() << "\n";
		}
	}
}

/*void removeUser(std::string t_id, map<std::string, User*> t_map) - removes a user with a given ID. By utilizing
the map function find, we first search whether a pair with the ID still exists in the map. We try and erase it and
catch an error when the ID can no longer be found.
*/
void User::removeUser(std::string t_id, std::map<std::string, User> &t_map){
	if (this->getUOrderStatus() == true) {
		throw "ERROR: user has an open demand and cannot be deleted!\n";
	}
	else {
		std::map<std::string, User>::iterator idFinder;
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
			std::cout << "ERROR while trying to remove the user: " << e.what() << "\n";
		}
	}
}
/*void printUser(): this function just prints out the user data on the console to check it*/
void User::printUser(){
	//create a legible printout for the user's gender
	std::string userGender = "";
	switch (this->getUGender()) {
	case gender::male:
		userGender = "male";
		break;
	case gender::female:
		userGender = "female";
		break;
	default:
		userGender = "other";
	}
	//and a printout for the order status!
	std::string orderStat = "";
	if (this->getUOrderStatus() == true) {
		orderStat = "Demand open";
	}
	else {
		orderStat = "No open demand";
	}

	std::cout << "USER " << this->getUserID() << "(nickname " << this->getUName() << "), age " << this->getUAge() << ", gender " << userGender << "\n" <<
	"contact information: phone number " << this->getUPhone() << ", mail address " << this->getUMail() << "\n" <<
		"accessibility options: " << this->getUAccess() << "\n" <<
		"order status: " << orderStat << "\n";
}

/*bool UexistsInDB(std::string t_uID, std::map<std::string, User> t_map): this function checks whether this user is in the database*/
bool existsInDB(std::string t_uID, std::map<std::string, User> t_map) {
	bool usrIsInDB = false;
	std::map<std::string, User>::iterator mapIT = t_map.begin();
	mapIT = t_map.find(t_uID);
	if (mapIT != t_map.end()) {
		usrIsInDB = true;
	}
	return usrIsInDB;
}

/*bool checkUOrderStatus(std::string t_userID, std::map<std::string, User> t_uTable) - checks whether the user with given ID already has an open demand or not
While creating the set-functions for the Demand class, I realized that I should probably check if a user already has an open demand or not.*/
bool checkUOrderStatus(std::string t_userID, std::map<std::string, User> t_uTable) {
	std::map<std::string, User>::iterator mapIT;
	mapIT = t_uTable.find(t_userID);
	if (mapIT != t_uTable.end()) {
		try {
			return mapIT->second.getUOrderStatus();
		}
		catch (std::exception &e) {
			std::cout << "Error while checking the user's order status: " << e.what() << "\n";
		}
	}
	else {
		throw "ERROR: User was not found in the database!\n";
	}
}