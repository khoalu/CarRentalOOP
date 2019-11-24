#ifndef I_BOOK_AND_RENT_H
#define I_BOOK_AND_RENT_H

#include <bitset>
#include <vector>

#include "Contract.h"

/**
    interface: Book and Rent: interact with client
*/

#define RETURN_BY_VEHICLE_ID 0
#define RETURN_BY_CONTRACT_ID 1

class IBookAndRent {
    /**
        Add a contract with this information

        @param clientInfo
        @param pickUpLocation
        @param beginDay
        @param endDay
        @param vehicleId

        @return new contract ID
    */
    virtual int makeNewContractWithClient(
                TClientInfo clientInfo,
                string pickUpLocation, TDateTime beginDay,
                string returnLocation, TDateTime endDay,
                int vehicleId
                ) = 0;

    /**
        Add a contract with this information

        @param flightTicket
        @param vehicleId

        @return new contract ID
    */
    virtual int makeNewContractWithClient(
                TFlightTicket flightTicket,
                int vehicleId
                ) = 0;

    /**
        Return a vehicle

        @param vehicleId: id of the vehicle returned by client
        @param type: 0 = return by vehicle id) , 1 = return by contract id

        @return none
    */
    virtual void returnVehicle(int id, int type) = 0;
};

#endif // I_BOOK_AND_RENT_H
