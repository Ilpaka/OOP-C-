#include <iostream>
#include <string>

class Car {
private:
    std::string brand;
    std::string model;
    const int year;
    std::string color;
    double mileage;
    double price;
public:
    Car(const std::string& brand, const std::string& model, int year, const std::string& color, double mileage, double price)
        : brand(brand), model(model), year(year), color(color), mileage(mileage), price(price) {}
    Car(const Car& other)
        : brand(other.brand), model(other.model), year(other.year), color(other.color), mileage(other.mileage), price(other.price) {}
    Car& operator=(const Car& other) {
        if (this == &other)
            return *this;
        color = other.color;
        mileage = other.mileage;
        price = other.price;
        return *this;
    }
    ~Car() {}
    std::string getBrand() const { return brand; }
    std::string getModel() const { return model; }
    int getYear() const { return year; }
    std::string getColor() const { return color; }
    double getMileage() const { return mileage; }
    double getPrice() const { return price; }
    void setColor(const std::string &newColor) { color = newColor; }
    void setMileage(double newMileage) { mileage = newMileage; }
    void setPrice(double newPrice) { price = newPrice; }
    friend std::ostream& operator<<(std::ostream& os, const Car& car);
};

std::ostream& operator<<(std::ostream& os, const Car& car) {
    os << "Марка: " << car.brand << "\n"
       << "Модель: " << car.model << "\n"
       << "Год выпуска: " << car.year << "\n"
       << "Цвет: " << car.color << "\n"
       << "Пробег: " << car.mileage << "\n"
       << "Стоимость: " << car.price << "\n";
    return os;
}

int main() {
    Car car("Toyota", "Camry", 2018, "Белый", 50000, 15000);
    std::cout << car << std::endl;
    car.setColor("Черный");
    car.setMileage(52000);
    car.setPrice(14500);
    std::cout << car << std::endl;
    Car carCopy = car;
    std::cout << carCopy << std::endl;
    return 0;
}
