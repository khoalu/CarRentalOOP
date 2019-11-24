#ifndef CONTRACT_H
#define CONTRACT_H

#include <string>
#include "Datetime.h"

using namespace std;

/**
    class TFlightTicket: contains information about a flight ticket
- attributes:
    name: name of the client
    locationFrom: where the client came from
    locationTo : where the client go to
    arriveDate: the date and time client go to
    leaveDate: the date and time client leave
**/

class TFlightTicket {
private:
    string name;
    string locationFrom, locationTo;
    TDateTime arriveDate, leaveDate;
public:
    TFlightTicket() {}
    TFlightTicket(string name, string locationFrom, string locationTo,
                  TDateTime arriveDate, TDateTime leaveDate = TDateTime()):
        name(name), locationFrom(locationFrom), locationTo(locationTo),
        arriveDate(arriveDate), leaveDate(leaveDate) {}
    ~TFlightTicket() {}

    string getName() {return name;}
    string getLocationFrom() {return locationFrom;}
    string getLocationTo() {return locationTo;}
    TDateTime getArriveDate() {return arriveDate;}
    TDateTime getLeaveDate() {return leaveDate;}
};

/**
    class TClientInfo: contains info of a specific client
- attributes:
    name: client's full name
    age: client's age. 18 <= age.
    IDnumber: client's identity card number
    license: client's driver license
    address: client's personal address
    phoneNumber: client's personal phone number
**/

class TClientInfo {
protected:
    string name;
    int age;
    int IDnumber;
    string license;
    string address;
    string phoneNumber;
public:
    TClientInfo() {}
    TClientInfo(string name): name(name) {}
    TClientInfo(string name, int age): name(name), age(age) {}
    TClientInfo(string name, int age, int IDnumber):
        name(name), age(age), IDnumber(IDnumber) {}

    TClientInfo(TFlightTicket flightTicket):
        name(flightTicket.getName()) {}

    void setPhoneNumber(string phoneNumber) {this->phoneNumber = phoneNumber;}
    string getPhoneNumber() {return phoneNumber;}
    void setAddress(string addr) {this->address = addr;}
    string getAddress() {return address;}
    void setName(string name) {this->name = name;}
    string getName() {return name;}
    void setLicense(string license) {this->license = license;}
    string getLicense() {return license;}
    void setIDnumber(int id) {this->IDnumber = id;}
    int getIDnumber() {return IDnumber;}
    void setAge(int age) {this->age = age; }
    int getAge() {return age;}
};

/**
    class ErrorType for finding contract in database
*/

enum class ErrorType {
    NOT_FOUND,
    NUM_OF_ERROR_TYPE
};


/**
    class Contract: contains information about a car rental contract
- attributes:
    static IDcount: this is the number of object(s) have been created of this class (index generator)

    id: object's ID
    clientInfo: information of a clients, described by class TClientInfo
    vehicleId: id of binding vehicle to this contract
    rentPeriod: rent period in days. 1 <= rentPeriod <= 30
    vehicleId: id of the rented vehicle

    rentalFee: rental fee (will not be set by self)
**/

class Contract
{
private:
    static int IDcount;

    int id;
    TClientInfo clientInfo;
    int vehicleId;
    TDateTime beginDay;
    int rentPeriod;
    string pickUpLocation;
    string returnLocation;

    int rentalFee; //   will be initialize after a new contract is added to database,
                   // not when the contract is created
public:
    Contract(): id(IDcount++) {}
    ~Contract() {}

    Contract(ErrorType error)
    {
        switch (error)
        {
        case ErrorType::NOT_FOUND:
            id = -1;
            break;
        default:
            break;
        }
    }

    Contract(TClientInfo clientInfo, int rentPeriod, int vehicleId,
             string pickUpLocation, string returnLocation):
        id(IDcount++), clientInfo(clientInfo),
        vehicleId(vehicleId), rentPeriod(rentPeriod),
        pickUpLocation(pickUpLocation), returnLocation(returnLocation)
        {
            beginDay = TDate(0,0,0);
        }

    Contract(TClientInfo clientInfo, TDateTime startDay, TDateTime endDay,
             int vehicleId, string pickUpLocation, string returnLocation):
        id(IDcount++), clientInfo(clientInfo), vehicleId(vehicleId), beginDay(startDay),
        pickUpLocation(pickUpLocation), returnLocation(returnLocation)
        {
            rentPeriod = endDay.date - startDay.date;
        }

    Contract(TFlightTicket flightTicket, int vehicleId, int rentPeriod):
        id(IDcount++), clientInfo(flightTicket), vehicleId(vehicleId),
        beginDay(flightTicket.getArriveDate()), rentPeriod(rentPeriod) {}

    Contract(TFlightTicket flightTicket, int vehicleId):
        id(IDcount++), clientInfo(flightTicket), vehicleId(vehicleId),
        beginDay(flightTicket.getArriveDate())
        {
            rentPeriod = flightTicket.getLeaveDate().date - flightTicket.getArriveDate().date;
        }
    //Contract(TClientInfo clientInfo, int rentPeriod, string vehicleType, string location); // will find the first car in storage with type "vehicleType"
    //Contract(TClientInfo clientInfo, TDate startDay, TDate endDay, string vehicleType, string location);

    int getId() {return id;}

    void setClientInfo(TClientInfo clientInfo) {this->clientInfo = clientInfo; }
    TClientInfo getClientInfo() {return clientInfo; }

    void setRentPeriod(int rentPeriod) {this->rentPeriod = rentPeriod; }
    int getRentPeriod() {return rentPeriod; }

    void setVehicleId(int id) {this->vehicleId = id; }
    int getVehicleId() {return vehicleId;}

    void setRentalFee(int rentalFee) {this->rentalFee = rentalFee;}
    int getRentalFee() {return rentalFee;}
    int getDamageFee();

    static int getNumOfContracts() {return IDcount;}
};
int Contract::IDcount = 0;

#endif // CONTRACT_H
