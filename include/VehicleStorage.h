#ifndef VEHICLE_STORAGE_H
#define VEHICLE_STORAGE_H

//#include <vector>

/**
    class VehicleStorage: contains information about all of the car in the company
- attributes:

    vector<Vehicle*>: a list contains all information about vehicles, defined by Vehicle* (pointer is used for polymorphism)

**/

class VehicleStorage {
    vector<Vehicle*> vehicles;
public:
    VehicleStorage();
    VehicleStorage(vector<Vehicle*>&); // add a set of vehicles via a vector

    void addVehicle(Vehicle*); // add a vehicle via its object pointer (could be a derived class of Vehicle)
    void addVehicle(string); // add a vehicle via its name/type
    void addVehicle(int); // add a vehicle via its id

    void addVehicle(Sedan&);
    void addVehicle(SUV&);
    void addVehicle(SportCar&);

    int getNumOfVehicles();
};

VehicleStorage::VehicleStorage() {}
VehicleStorage::VehicleStorage(vector<Vehicle*>& v): vehicles(v) {}

VehicleStorage::getNumOfVehicles() {return this->vehicles.size();}
void VehicleStorage::addVehicle(Vehicle* pVehicle) {this->vehicles.push_back(pVehicle);}

void VehicleStorage::addVehicle(Sedan& sedan) {this->vehicles.push_back(&sedan);}
void VehicleStorage::addVehicle(SUV &suv) {this->vehicles.push_back(&suv);}
void VehicleStorage::addVehicle(SportCar& sport) {this->vehicles.push_back(&sport);}

#endif // VEHICLE_STORAGE_H
