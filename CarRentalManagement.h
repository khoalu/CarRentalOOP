#ifndef CAR_RENTAL_MANAGEMENT_H
#define CAR_RENTAL_MANAGEMENT_H

#include <vector>

#include "Vehicle.h"
#include "Contract.h"
#include "IBookAndRent.h"

class CarRentalManagement: public IBookAndRent {
private:
    std::vector<Vehicle*> vehicles;
    std::vector<Contract> contracts;

    int counting[(int)VehicleType::NUM_OF_VEHICLE_TYPE];

public:
    CarRentalManagement()
    {
        for(int i = 0; i < (int)VehicleType::NUM_OF_VEHICLE_TYPE; i++)
            counting[i] = 0;
    }
    ~CarRentalManagement()
    {
        for(auto &x: vehicles) delete x;
        vehicles.clear();
    }

    int addVehicle(Vehicle*); // add a vehicle via its object pointer (could be a derived class of Vehicle)
    int addVehicle(VehicleType); // add a vehicle via its name/type
    int addVehicle(VehicleType, int color, float damage, bool available);

    int addContract(Contract&);

    int getNumOfVehicles();
    int getNumOfContracts();

    int getNumOf(VehicleType type) {return counting[(int)type];}

    void serviceFleet();
    Vehicle* getVehicleBasedOnId(int id)
    {
        for(auto &v: vehicles)
        {
            if (v->getId() == id) return v;
        }
        return nullptr;
    }

    Contract getContracBaseOnId(int id)
    {
        for(auto &v: contracts)
        {
            if (v.getId() == id) return v;
        }
        return Contract(ErrorType::NOT_FOUND);
    }

    void showAvailableVehicle(
            std::bitset<(int)VehicleType::NUM_OF_VEHICLE_TYPE> &vehicleFlag
            );

    int makeNewContractWithClient(
                TClientInfo clientInfo,
                string pickUpLocation, TDateTime beginDay,
                string returnLocation, TDateTime endDay,
                int vehicleId
                );

    int makeNewContractWithClient(
                TFlightTicket flightTicket,
                int vehicleId
                );

    void returnVehicle(int id, int type);
};

int CarRentalManagement::getNumOfVehicles() {return this->vehicles.size();}
int CarRentalManagement::getNumOfContracts() {return this->contracts.size();}
int CarRentalManagement::addVehicle(Vehicle* pVehicle)
{
    this->vehicles.push_back(pVehicle);
    this->counting[(int)pVehicle->getType()]++;
    return pVehicle->getId();
}

int CarRentalManagement::addVehicle(VehicleType vehicleType)
{
    switch(vehicleType)
    {
    case VehicleType::SEDAN:
        return addVehicle(new Sedan());
        break;
    case VehicleType::SPORT_CAR:
        return addVehicle(new SportCar());
        break;
    case VehicleType::SUV:
        return addVehicle(new SUV());
        break;
    default:
        break;
    }
    return -1;
}

int CarRentalManagement::addVehicle(VehicleType vehicleType, int color,
                                    float damage=0, bool available=0)
{
    switch(vehicleType)
    {
    case VehicleType::SEDAN:
        return addVehicle(new Sedan(color, damage, available));
        break;
    case VehicleType::SPORT_CAR:
        return addVehicle(new SportCar(color, damage, available));
        break;
    case VehicleType::SUV:
        return addVehicle(new SUV(color, damage, available));
        break;
    default:
        break;
    }
    return -1;
}


int CarRentalManagement::addContract(Contract& contract) {this->contracts.push_back(contract); return contract.getId();}
void CarRentalManagement::serviceFleet()
{
    for(auto &v : vehicles)
    {
        v->serviceEngine(v->getMileage());
        v->serviceTransmission(v->getMileage());
        v->serviceTires(v->getMileage());

        v->printServiceHistory();
    }
}
void CarRentalManagement::showAvailableVehicle(
            std::bitset<(int)VehicleType::NUM_OF_VEHICLE_TYPE> &vehicleFlag
            )
{
    printf("%4s%15s%10s%10s\n\n","ID","Type","Seats","Color");
    for(Vehicle* &pVec: vehicles)
    {
        if (vehicleFlag[(int)pVec->getType()] && pVec->getAvailable())
        {
            printf("%4d%15s%10d%10d\n",
                    pVec->getId(), pVec->getTypeName().c_str(),
                    pVec->getNumOfSeats(), pVec->getColor());
//            cout << pVec->getId() << "\t";
//            cout << pVec->getTypeName() << "\t\t\t";
//            cout << pVec->getNumOfSeats() << "\t";
//            cout << pVec->getColor() << "\t";
//            cout << "\n";
        }
    }
}

int CarRentalManagement::makeNewContractWithClient(
        TClientInfo clientInfo,
        string pickUpLocation, TDateTime beginDay,
        string returnLocation, TDateTime endDay,
        int vehicleId
        )
{
    Contract contract(clientInfo, beginDay.date, endDay.date, vehicleId, pickUpLocation, returnLocation);
    Vehicle* pVehicle = getVehicleBasedOnId(vehicleId);
    pVehicle->setAvailable(false);
    pVehicle->setContractId(contract.getId());
    contract.setRentalFee(pVehicle->getPrice() * contract.getRentPeriod());
    return addContract(contract);
}

int CarRentalManagement::makeNewContractWithClient(
        TFlightTicket flightTicket,
        int vehicleId
        )
{
    return makeNewContractWithClient(flightTicket,
                                     flightTicket.getLocationTo(),
                                     flightTicket.getArriveDate(),
                                     flightTicket.getLocationTo(),
                                     flightTicket.getLeaveDate(),
                                     vehicleId
                                     );
}

void CarRentalManagement::returnVehicle(int id, int type)
{
    Vehicle* pVehicle;
    switch (type)
    {
    case RETURN_BY_CONTRACT_ID:
        pVehicle = getVehicleBasedOnId(getContracBaseOnId(id).getVehicleId());
        break;
    case RETURN_BY_VEHICLE_ID:
        pVehicle = getVehicleBasedOnId(id);
    default:
        throw runtime_error("?");
        break;
    }
    pVehicle->setAvailable(true);

    ///do something with contract here (get the damage fee, ...)
}


#endif // CAR_RENTAL_MANAGEMENT_H
