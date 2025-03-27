#include <iostream>
#include <vector>
#include <string>

class Car {
protected:
    std::string brand;
    std::string model;
    int year;
    std::string color;
    double mileage;
    double price;
public:
    Car(const std::string& b, const std::string& m, int y,
        const std::string& c, double ml, double p)
        : brand(b), model(m), year(y), color(c), mileage(ml), price(p) {}
    Car(const Car& other)
        : brand(other.brand), model(other.model), year(other.year),
          color(other.color), mileage(other.mileage), price(other.price) {}
    Car& operator=(const Car& other) {
        if (this == &other) return *this;
        brand = other.brand;
        model = other.model;
        year = other.year;
        color = other.color;
        mileage = other.mileage;
        price = other.price;
        return *this;
    }
    virtual ~Car() {}
};

class Package {
private:
    std::string name;
    double weight;
public:
    Package(const std::string& n, double w) : name(n), weight(w) {}
    double getWeight() const { return weight; }
};

class Truck final : public Car {
private:
    double maxPayload;
    std::vector<Package> packages;
public:
    Truck(const std::string& b, const std::string& m, int y, const std::string& c,
          double ml, double p, double mp)
        : Car(b, m, y, c, ml, p), maxPayload(mp) {}
    Truck(const Truck& other)
        : Car(other), maxPayload(other.maxPayload), packages(other.packages) {}
    Truck& operator=(const Truck& other) {
        if (this == &other) return *this;
        Car::operator=(other);
        maxPayload = other.maxPayload;
        packages = other.packages;
        return *this;
    }
    ~Truck() {}
    void addPackage(const Package& pkg) { packages.push_back(pkg); }
    bool isReady() const {
        double totalWeight = 0;
        for (const auto& pkg : packages) totalWeight += pkg.getWeight();
        return totalWeight <= maxPayload;
    }
};

int main() {
    Truck t("Volvo", "FH16", 2020, "White", 100000, 50000, 20000);
    t.addPackage(Package("Box1", 5000));
    t.addPackage(Package("Box2", 7000));
    std::cout << (t.isReady() ? "Ready" : "Not ready") << std::endl;
    return 0;
}
