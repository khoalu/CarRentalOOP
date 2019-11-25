#include <iostream>
#include <vector>

/// FOR DEBUG PURPOSE ONLY
#define CAN_SEE_SERVICE_HISTORY

#include "CarRentalManagement.h"

using namespace std;

class TestClass
{
public:
    void testOverloading()
    {
        /**
            Overloading contract constructors
        */
        Contract *contract;
        contract = new Contract();
        delete contract;

        contract = new Contract(ErrorType::NOT_FOUND);
        delete contract;

        contract = new Contract(TClientInfo("Khoa",19), TDate(2019,1,1),
                                TDate(2019,2,2), 0, "HCM", "HCM");
        delete contract;

        contract = new Contract(TClientInfo("Khoa"), 20, 0, "HCM", "HCM");
        delete contract;

        contract = new Contract(
                    TFlightTicket("Khoa","HCM","HaNoi",
                                  TDateTime("2019-12-01T00:00:00Z"),
                                  TDateTime("2019-12-25T00:00:00Z")
                                  ),
                    0);
        delete contract;

        contract = new Contract(
                    TFlightTicket("Khoa","HCM","HaNoi",
                                  TDateTime("2019-12-01T00:00:00Z"),
                                  TDateTime("2019-12-25T00:00:00Z")
                                  ),
                    0, 20);
        delete contract;

        /**
            Overloading add vehicles method
        */
        CarRentalManagement manager;

        manager.addVehicle(new Sedan());
        manager.addVehicle(new Sedan(19));
        manager.addVehicle(VehicleType::SUV);
        manager.addVehicle(VehicleType::SUV, 19);
    }

    void testInheritanceAndComposition()
    {
        CarRentalManagement manager;
        manager.addVehicle(new Sedan());
        manager.addVehicle(new SUV());
        manager.addVehicle(new SportCar());

        Vehicle *pVehicle = manager.getVehicleBasedOnId(0);
        ServiceHistory& sh = pVehicle->getServiceHistory();
        sh.addService(new MaintenanceJob(MaintenanceJobType::FLUID_CHANGE,
                                         100, 2000));
        sh.addService(new MaintenanceJob(MaintenanceJobType::OIL_CHANGE,
                                         300, 5000));

        pVehicle = manager.getVehicleBasedOnId(1);
        ServiceHistory& sh2 = pVehicle->getServiceHistory();
        sh2.addService(new MaintenanceJob(MaintenanceJobType::TRANSMISSION_OVERHAUL,
                                         100, 1000));
        sh2.addService(new MaintenanceJob(MaintenanceJobType::TIRE_ADJUSTMENT,
                                         300, 500));

        manager.serviceFleet();
    }

    void testRenting()
    {
        /**
            REMINDER IF SOME KIND OF USER INTERFACE IS IMPLEMENTED:
            Please take these steps as a guideline:
            1. collect location, date and time of pick up day and return day
            2. show available stock
                using CarRentalManagement::showAvailableVehicle(...)
            3. collect client's information (name, age, ...)
                with TClientInfo class
            4. Create a new contract
                using CarRentalManagement::makeContractWithClient(...)
            5. Check out phase, display rental fee by
                using Contract::getRentalFee()
            6. When contract is accepted, displaying the contract id
                using Contract::getId()

            7. When vehicle is returned by vehicle id or contract id
                ,use CarRentalManagement::returnVehicle(...)
        */

        CarRentalManagement manager;

        // BEGIN adding some vehicles
        manager.addVehicle(new Sedan(20));
        manager.addVehicle(new SUV(10));
        manager.addVehicle(new SportCar(15));
        manager.addVehicle(VehicleType::SEDAN);
        manager.addVehicle(VehicleType::SUV);
        // END adding some vehicles

        // show all available vehicle
        std::bitset<(int)VehicleType::NUM_OF_VEHICLE_TYPE> bitMask;
        bitMask = ~bitMask;
        manager.showAvailableVehicle(bitMask);

        // BEGIN data collected from client
        TClientInfo clientInfo("Khoa",19,1852112);
        string pickUpLocation = "HCMCity";
        TDateTime beginDay("2019-12-01T00:00:00Z");

        string returnLocation = "HCMCity";
        TDateTime endDay("2019-12-25T00:00:00Z");

        int vehicleId = 1;
        // END data collected from client

        int clientContractId; // this contract id will be given to client
        clientContractId = manager.makeNewContractWithClient(clientInfo, pickUpLocation, beginDay, returnLocation, endDay, vehicleId);
        cout << clientContractId << "\n-------------------------\n";

        // try to print out some contract information to confirm
        Contract contract = manager.getContracBaseOnId(clientContractId);
        cout << "contract id = " << contract.getId() << "\n";
        cout << "vehicle id = " << contract.getVehicleId() << "\n";
        cout << "client name = " << contract.getClientInfo().getName() << "\n";
        cout << "rent period = " << contract.getRentPeriod() << "\n";
        cout << "base rate = " << (*manager.getVehicleBasedOnId(manager.getContracBaseOnId(clientContractId).getVehicleId())).getPrice() << "\n";
        cout << "total fee = " << contract.getRentalFee() << "\n";

        // print available stuff after we get a vehicle
        cout << "\n--------------------\n";
        cout << "Available vehicle after client rents a vehicle\n";
        manager.showAvailableVehicle(bitMask);

        // print available stuff after we return a vehicle
    //    manager.returnVehicle(clientContractId, RETURN_BY_CONTRACT_ID);
        manager.returnVehicle(clientContractId, 2);
        cout << "\n--------------------\n";
        cout << "Available vehicle after client returns a vehicle\n";
        manager.showAvailableVehicle(bitMask);
    }

    void testOperatorOverloading()
    {
        ServiceHistory sH;
        int mileage = 200;
        sH.addService(new MaintenanceJob(MaintenanceJobType::OIL_CHANGE, mileage + 50, 200));
        sH.addService(new MaintenanceJob(MaintenanceJobType::FLUID_CHANGE, mileage+50, 200));
        MaintenanceJob a, b;
        a = *sH[0];
        b = *sH[1];
        cout << b - a << "\n";
        if (a < b)
            cout << "a < b";
        else if (a > b)
            cout << "a > b";
        else
            cout << "a == b";
    }

    void testSerialize()
    {
        Vehicle* pVehicle = new SUV(20);

        ServiceHistory& sh = pVehicle->getServiceHistory();
        sh.addService(new MaintenanceJob(MaintenanceJobType::FLUID_CHANGE,
                                         10000, 2000));
        sh.addService(new MaintenanceJob(MaintenanceJobType::OIL_CHANGE,
                                         30000, 5000));
        sh.addService(new MaintenanceJob(MaintenanceJobType::TRANSMISSION_OVERHAUL,
                                         50000, 7000));

        vector<Document*> arr;
        string filePath = "output.json";
        pVehicle->printJsonToFile(filePath);
        cout << "Done writing to " << filePath << "\n";
    }
};

int main()
{
    TestClass tc;
    int testCase = 3;
    switch (testCase)
    {
    case 0:
        tc.testOverloading();
        break;
    case 1:
        tc.testInheritanceAndComposition();
        break;
    case 2:
        tc.testOperatorOverloading();
        break;
    case 3:
        tc.testSerialize();
        break;
    }

    return 0;
}

/// OLD CODE
//    CarRentalManagement manager;
//
//    manager.addVehicle(VehicleType::SUV);
//    //manager.addVehicle(new Sedan());
//
//    Vehicle* pVehicle = manager.getVehicleBasedOnId(0);
//
//
//
//    pVehicle->setMileage(10000);
////
////    Contract contract;
////    manager.addContract(contract);
////
//    manager.serviceFleet();
//    vector<Document*> arr;
//    pVehicle->printJsonToFile(pVehicle->getDomObject(arr), arr, "output.json");
////    cout << manager.getNumOfVehicles() << " " << manager.getNumOfContracts();
////    cout << "\n";
////    cout << "Sedans: " << manager.getNumOf(VehicleType::SEDAN) << "\n";
////    cout << "SUVs: " << manager.getNumOf(VehicleType::SUV) << "\n";
////    cout << "Sports: " << manager.getNumOf(VehicleType::SPORT_CAR) << "\n";
//
//    return 0;
