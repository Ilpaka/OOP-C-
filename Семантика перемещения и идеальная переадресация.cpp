#include <iostream>
#include <string>
#include <vector>

class Vehicle {
public:
    virtual void move(double time) = 0;
    virtual ~Vehicle() {}
};

class LandVehicle : public Vehicle {
protected:
    double currentVelocity;
    double distance;
public:
    LandVehicle(double velocity, double dist = 0)
        : currentVelocity(velocity), distance(dist) {}
    LandVehicle(const LandVehicle& other)
        : currentVelocity(other.currentVelocity), distance(other.distance) {}
    LandVehicle(LandVehicle&& other) noexcept
        : currentVelocity(other.currentVelocity), distance(other.distance) {
        other.currentVelocity = 0;
        other.distance = 0;
    }
    LandVehicle& operator=(const LandVehicle& other) {
        if (this == &other) return *this;
        currentVelocity = other.currentVelocity;
        distance = other.distance;
        return *this;
    }
    LandVehicle& operator=(LandVehicle&& other) noexcept {
        if (this == &other) return *this;
        currentVelocity = other.currentVelocity;
        distance = other.distance;
        other.currentVelocity = 0;
        other.distance = 0;
        return *this;
    }
    double getDistance() const { return distance; }
};

class Car : public LandVehicle {
protected:
    std::string brand;
    std::string model;
    int year;
    std::string color;
    double mileage;
    double price;
public:
    Car(const std::string& b, const std::string& m, int y,
        const std::string& c, double ml, double p, double velocity)
        : LandVehicle(velocity), brand(b), model(m), year(y),
          color(c), mileage(ml), price(p) {}
    Car(const Car& other)
        : LandVehicle(other), brand(other.brand), model(other.model),
          year(other.year), color(other.color), mileage(other.mileage),
          price(other.price) {}
    Car(Car&& other) noexcept
        : LandVehicle(std::move(other)),
          brand(std::move(other.brand)), model(std::move(other.model)),
          year(other.year), color(std::move(other.color)),
          mileage(other.mileage), price(other.price) {}
    Car& operator=(const Car& other) {
        if (this == &other) return *this;
        LandVehicle::operator=(other);
        brand = other.brand;
        model = other.model;
        year = other.year;
        color = other.color;
        mileage = other.mileage;
        price = other.price;
        return *this;
    }
    Car& operator=(Car&& other) noexcept {
        if (this == &other) return *this;
        LandVehicle::operator=(std::move(other));
        brand = std::move(other.brand);
        model = std::move(other.model);
        year = other.year;
        color = std::move(other.color);
        mileage = other.mileage;
        price = other.price;
        return *this;
    }
    virtual ~Car() {}
    void move(double time) override {
        distance += currentVelocity * time;
    }
};

class Truck final : public Car {
    double maxPayload;
    std::vector<std::string> packages;
public:
    Truck(const std::string& b, const std::string& m, int y,
          const std::string& c, double ml, double p, double velocity, double maxP)
        : Car(b, m, y, c, ml, p, velocity), maxPayload(maxP) {}
    Truck(const Truck& other)
        : Car(other), maxPayload(other.maxPayload), packages(other.packages) {}
    Truck(Truck&& other) noexcept
        : Car(std::move(other)), maxPayload(other.maxPayload), packages(std::move(other.packages)) {}
    Truck& operator=(const Truck& other) {
        if (this == &other) return *this;
        Car::operator=(other);
        maxPayload = other.maxPayload;
        packages = other.packages;
        return *this;
    }
    Truck& operator=(Truck&& other) noexcept {
        if (this == &other) return *this;
        Car::operator=(std::move(other));
        maxPayload = other.maxPayload;
        packages = std::move(other.packages);
        return *this;
    }
    ~Truck() {}
    void move(double time) override {
        distance += currentVelocity * time;
    }
};

class Motorcycle : public LandVehicle {
public:
    Motorcycle(double velocity) : LandVehicle(velocity) {}
    Motorcycle() : LandVehicle(0) {}
    Motorcycle(const Motorcycle& other) : LandVehicle(other) {}
    Motorcycle(Motorcycle&& other) noexcept : LandVehicle(std::move(other)) {}
    Motorcycle& operator=(const Motorcycle& other) {
        if (this == &other) return *this;
        LandVehicle::operator=(other);
        return *this;
    }
    Motorcycle& operator=(Motorcycle&& other) noexcept {
        if (this == &other) return *this;
        LandVehicle::operator=(std::move(other));
        return *this;
    }
    ~Motorcycle() {}
    void move(double time) override {
        distance += currentVelocity * time;
    }
};

int main() {
    Car car1("Toyota", "Camry", 2018, "White", 50000, 15000, 60);
    Car car2 = std::move(car1);
    Truck truck1("Volvo", "FH16", 2020, "White", 100000, 50000, 50, 20000);
    Truck truck2 = std::move(truck1);
    Motorcycle moto1(80);
    Motorcycle moto2;
    moto2 = std::move(moto1);
    car2.move(1.0);
    truck2.move(1.0);
    moto2.move(1.0);
    std::cout << "Car distance: " << car2.getDistance() << "\n";
    std::cout << "Truck distance: " << truck2.getDistance() << "\n";
    std::cout << "Motorcycle distance: " << moto2.getDistance() << "\n";
    return 0;
}
