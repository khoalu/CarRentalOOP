#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

#include "VehiclePrimitives.h"
#include "ServiceHistory.h"

using namespace std;

enum class VehicleType {
    SEDAN,
    SUV,
    SPORT_CAR,
    NUM_OF_VEHICLE_TYPE
};

/**
    @class Vehicle: contains information about a specific kind of Vehicle

- attributes:
    static IDcount: this is the number of object(s) have been created of this class (index generator)

    id: object's ID
    type: vehicle's type (enum type)
    typeName: vehicle's type(string type)
    numOfSeats: number of seats of the vehicle
    color: vehicle's color. 0x000000 <= color <= 0xFFFFFF
    price: base price for hiring (per day)
    damage: vehicle's damage status. 0 <= damage <= 1. 0 indicates the car is in its best state and 1 indicates highly damaged car(cannot use anymore)
    available: true if this vehicle is ready for hiring, false otherwise.
    contractId: the id of this vehicle's binding contract

    mileage: how many miles this vehicle has ran
    serviceHistory: how the car has been changed (engine, oil, ...)
**/

class Vehicle: public IJsonHandle {

    static int IDcount;
protected:
    int id;
    string typeName;
    VehicleType type;
    int numOfSeats;
    int color;
    int price;
    float damage;
    bool available;

    int mileage;
    ServiceHistory serviceHistory;

    int contractId;

public:
    Vehicle(string typeName, VehicleType type, int numOfSeats, int color, int price, float damage=0,
            bool available=true, int mileage=0):
        id(IDcount++), typeName(typeName), type(type), numOfSeats(numOfSeats), color(color),
        price(price), damage(damage), available(available), mileage(mileage), contractId(-1) {}
    virtual ~Vehicle() {}

    int getId() {return id;}

    void setContractId(int contractId) {this->contractId = contractId;}
    int getContractId() {return contractId;}

    string getTypeName() {return typeName;}
    VehicleType getType() {return type;}

    int getColor() {return color;}
    int getNumOfSeats() {return numOfSeats;}

    void setPrice(int price) {this->price = price;}
    int getPrice() {return price;}

    void setDamage(float damage) {this->damage = damage;}
    float getDamage() {return damage;}

    void setMileage(int mileage) {this->mileage = mileage;}
    int getMileage() {return mileage;}

    void setAvailable(bool available) {this->available = available;}
    bool getAvailable() {return available; }

    void printServiceHistory()
    {
        serviceHistory.printHistory();
    }

    #ifdef CAN_SEE_SERVICE_HISTORY
    /// for debug purpose only

    ServiceHistory& getServiceHistory() {return serviceHistory;}

    #endif // CAN_SEE_SERVICE_HISTORY

    static int getNumOfVehicles() {return IDcount;}

    virtual void serviceEngine(int mileage) = 0;
    virtual void serviceTransmission(int mileage) = 0;
    virtual void serviceTires(int mileage) = 0;

    Document* getDomObject(vector<Document*> &arr)
    {
        Document *d = new Document();
        d->SetObject();
        auto &a = d->GetAllocator();
        d->AddMember("id", id , a);
        d->AddMember("typeName", StringRef(typeName.c_str()), a);
        d->AddMember("price", price, a);
        d->AddMember("seats", numOfSeats, a);
        d->AddMember("damage", damage, a);
        d->AddMember("available", available, a);
        d->AddMember("mileage", mileage, a);
        d->AddMember("serviceHistory", *(serviceHistory.getDomObject(arr)), a);
        d->AddMember("contractId", contractId, a);
//        serviceHistory.printJsonToFile(serviceHistory.getDomObject(), "output_serviceHistory.json");
        arr.push_back(d);
        return d;
    }

};
int Vehicle::IDcount = 0;

class SUV: public Vehicle {
protected:
    int temp;
public:
    SUV(int color=0, float damage=0, bool available=true):
        Vehicle("SUV", VehicleType::SUV, 7, color, 20, damage, available) {}
    void serviceEngine(int mileage)
    {
        cout << "Service Engine - SUV\n";
        //serviceHistory.addService(new TireChange(TDateTime(TDate(2000,12,3)), TDateTime(TDate(2000,12,5))));


//        serviceHistory.addService(new MaintenanceJob(MaintenanceJobType::OIL_CHANGE, this->mileage, 200));;
//        serviceHistory.addService(new MaintenanceJob(MaintenanceJobType::FLUID_CHANGE, this->mileage+200, 500));
//        serviceHistory.addService(new MaintenanceJob(MaintenanceJobType::FLUID_CHANGE, this->mileage+400, 100));
    }
    void serviceTransmission(int mileage)
    {
        cout << "Service Transmission - SUV\n";
    }
    void serviceTires(int mileage)
    {
        cout << "Service Tires - SUV\n";

        if (mileage >= 500)
        {
           // serviceHistory.addService((new TireChange()));
        }
    }
};

class SportCar: public Vehicle {
protected:
    int maxSpeed;
public:
    SportCar(int color=0, float damage=0, bool available=true):
        Vehicle("Sport Car", VehicleType::SPORT_CAR, 4, color, 15, damage, available)
        {
            this->maxSpeed = 200;
        }
    void serviceEngine(int mileage)
    {
        cout << "Service Engine - Sport\n";
    }
    void serviceTransmission(int mileage)
    {
        cout << "Service Transmission - Sport\n";
    }
    void serviceTires(int mileage)
    {
        cout << "Service Tires - Sport\n";
    }
};

class Sedan: public Vehicle {
protected:
    Engine engine;
    PassengerBox passengerBox;
    Cargo cargo;
public:
    Sedan(int color=0, float damage=0, bool available=true):
        Vehicle("Sedan", VehicleType::SEDAN, 5, color, 20, damage, available)
        {
            engine = Engine(20);
            passengerBox = PassengerBox(3, 4, 5);
            cargo = Cargo(3, 4, 5, 20, 15);
        }
    void serviceEngine(int mileage)
    {
        cout << "Service Engine - Sedan\n";
    }
    void serviceTransmission(int mileage)
    {
        cout << "Service Transmission - Sedan\n";
    }
    void serviceTires(int mileage)
    {
        cout << "Service Tires - Sedan\n";
    }
};

#endif // VEHICLE_H
