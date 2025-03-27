#include <iostream>
#include <string>
#include <vector>
#include <random>

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
    LandVehicle(double velocity, double dist = 0) : currentVelocity(velocity), distance(dist) {}
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
    Car(const std::string& b, const std::string& m, int y, const std::string& c, double ml, double p, double velocity)
        : LandVehicle(velocity), brand(b), model(m), year(y), color(c), mileage(ml), price(p) {}
    Car(const Car& other)
        : LandVehicle(other.currentVelocity, other.distance), brand(other.brand), model(other.model), year(other.year),
          color(other.color), mileage(other.mileage), price(other.price) {}
    Car& operator=(const Car& other) {
        if (this == &other) return *this;
        currentVelocity = other.currentVelocity;
        distance = other.distance;
        brand = other.brand;
        model = other.model;
        year = other.year;
        color = other.color;
        mileage = other.mileage;
        price = other.price;
        return *this;
    }
    virtual ~Car() {}
    void move(double time) override {
        distance += currentVelocity * time;
    }
};

class Package {
    std::string name;
    double weight;
public:
    Package(const std::string& n, double w) : name(n), weight(w) {}
    double getWeight() const { return weight; }
};

class Truck final : public Car {
    double maxPayload;
    std::vector<Package> packages;
public:
    Truck(const std::string& b, const std::string& m, int y, const std::string& c, double ml, double p, double velocity, double maxP)
        : Car(b, m, y, c, ml, p, velocity), maxPayload(maxP) {}
    Truck(const Truck& other) : Car(other), maxPayload(other.maxPayload), packages(other.packages) {}
    Truck& operator=(const Truck& other) {
        if (this == &other) return *this;
        Car::operator=(other);
        maxPayload = other.maxPayload;
        packages = other.packages;
        return *this;
    }
    ~Truck() {}
    void move(double time) override {
        distance += currentVelocity * time;
    }
    bool isReady() const {
        double total = 0;
        for (const auto &pkg : packages) total += pkg.getWeight();
        return total <= maxPayload;
    }
    void addPackage(const Package &pkg) {
        packages.push_back(pkg);
    }
};

class Motorcycle : public LandVehicle {
public:
    Motorcycle(double velocity) : LandVehicle(velocity) {}
    void move(double time) override {
        distance += currentVelocity * time;
    }
};

class Bicycle : public LandVehicle {
public:
    Bicycle(double velocity) : LandVehicle(velocity) {}
    void move(double time) override {
        distance += currentVelocity * time;
    }
};

class AirVehicle : public Vehicle {
protected:
    double currentVelocity;
    double distance;
public:
    AirVehicle(double velocity) : currentVelocity(velocity), distance(0) {}
    double getDistance() const { return distance; }
    void move(double time) override {
        distance += currentVelocity * time;
    }
};

class Airplane : public AirVehicle {
public:
    Airplane(double velocity) : AirVehicle(velocity) {}
};

class WaterVehicle : public Vehicle {
protected:
    double currentVelocity;
    double distance;
public:
    WaterVehicle(double velocity) : currentVelocity(velocity), distance(0) {}
    double getDistance() const { return distance; }
    void move(double time) override {
        distance += currentVelocity * time;
    }
};

class Boat : public WaterVehicle {
public:
    Boat(double velocity) : WaterVehicle(velocity) {}
};

int main() {
    std::vector<LandVehicle*> race;
    race.push_back(new Car("Toyota", "Camry", 2018, "White", 50000, 15000, 60));
    Truck* truck = new Truck("Volvo", "FH16", 2020, "White", 100000, 50000, 50, 20000);
    truck->addPackage(Package("Box1", 5000));
    truck->addPackage(Package("Box2", 7000));
    race.push_back(truck);
    race.push_back(new Motorcycle(80));
    race.push_back(new Bicycle(20));
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> timeDist(0.1, 1.0);
    for (int i = 0; i < 5; i++) {
        double t = timeDist(gen);
        for (auto v : race) v->move(t);
    }
    for (auto v : race) std::cout << v->getDistance() << std::endl;
    for (auto v : race) delete v;
    
    AirVehicle* airplane = new Airplane(300);
    WaterVehicle* boat = new Boat(40);
    airplane->move(2);
    boat->move(2);
    std::cout << "Airplane distance: " << airplane->getDistance() << std::endl;
    std::cout << "Boat distance: " << boat->getDistance() << std::endl;
    delete airplane;
    delete boat;
    return 0;
}
