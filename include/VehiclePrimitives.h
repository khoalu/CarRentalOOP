#ifndef VEHICLE_PRIMITIVES_H
#define VEHICLE_PRIMITIVES_H



struct Engine {
protected:
    int power; // measures in watts
public:
    Engine(int power=0): power(power) {}
};

struct PassengerBox {
protected:
    float length, width, height;// sizes of the passenger box in meters
public:
    PassengerBox(float length=0.0f, float width=0.0f, float height=0.0f): length(length), width(width), height(height) {}
};

struct Cargo {
protected:
    float length, width, height;// sizes of the cargo in meters
    float maxWeight, recommendWeight; // maximum weight that this cargo can hold and the recommend weight
public:
    Cargo() {}
    Cargo(float length, float width, float height): length(length), width(width), height(height) {}
    Cargo(float length, float width, float height, float maxWeight, float recommendWeight):Cargo(length, width, height)
    {
        this->maxWeight = maxWeight;
        this->recommendWeight = recommendWeight;
    }
};
#endif // VEHICLE_PRIMITIVES
