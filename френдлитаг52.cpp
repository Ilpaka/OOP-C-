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
    void move(double time) override { distance += currentVelocity * time; }
};

class Package;
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
    void move(double time) override { distance += currentVelocity * time; }
    bool isReady() const {
        double total = 0;
        for (const auto &pkg : packages) total += pkg.getWeight();
        return total <= maxPayload;
    }
    void addPackage(const std::string &name, double weight);
};

class Package {
private:
    std::string name;
    double weight;
    Package(const std::string& n, double w) : name(n), weight(w) {}
    friend class Truck;
    friend std::ostream& operator<<(std::ostream& os, const Package& pkg);
public:
    Package() = delete;
    double getWeight() const { return weight; }
};

std::ostream& operator<<(std::ostream& os, const Package& pkg) {
    os << "Package: " << pkg.name << ", Weight: " << pkg.weight;
    return os;
}

void Truck::addPackage(const std::string &name, double weight) {
    packages.push_back(Package(name, weight));
}

int main() {
    Truck t("Volvo", "FH16", 2020, "White", 100000, 50000, 50, 20000);
    t.addPackage("Box1", 5000);
    t.addPackage("Box2", 7000);
    std::cout << (t.isReady() ? "Ready" : "Not ready") << std::endl;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> timeDist(0.1, 1.0);
    for (int i = 0; i < 5; i++) {
        double tmove = timeDist(gen);
        t.move(tmove);
    }
    std::cout << "Distance: " << t.getDistance() << std::endl;
    return 0;
}
