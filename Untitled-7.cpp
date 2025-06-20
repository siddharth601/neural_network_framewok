#include <iostream>
using namespace std;
class Vehicle {
protected:
    string brand;
    string model;
    int year;

public:
    virtual void input() {
        cout << "Enter Brand: ";
        cin >> brand;
        cout << "Enter Model: ";
        cin >> model;
        cout << "Enter Year: ";
        cin >> year;
    }

    virtual void display() const {
        cout << "Brand: " << brand << ", Model: " << model << ", Year: " << year << endl;
    }

    virtual ~Vehicle() {}
};
class Car : public Vehicle {
private:
    int doors;

public:
    void input() override {
        Vehicle::input();
        cout << "Enter number of doors: ";
        cin >> doors;
    }

    void display() const override {
        Vehicle::display();
        cout << "Car has " << doors << " doors." << endl;
    }
};
class Bike : public Vehicle {
private:
    bool hasGear;

public:
    void input() override {
        Vehicle::input();
        char choice;
        cout << "Does the bike have gears? (y/n): ";
        cin >> choice;
        hasGear = (choice == 'y' || choice == 'Y');
    }

    void display() const override {
        Vehicle::display();
        cout << "Bike has " << (hasGear ? "gears." : "no gears.") << endl;
    }
};
class Truck : public Vehicle {
private:
    double loadCapacity;

public:
    void input() override {
        Vehicle::input();
        cout << "Enter load capacity in tons: ";
        cin >> loadCapacity;
    }

    void display() const override {
        Vehicle::display();
        cout << "Truck Load Capacity: " << loadCapacity << " tons" << endl;
    }
};

int main() {
    Vehicle* v;

    cout << "\n-- Car Info --\n";
    Car car;
    v = &car;
    v->input();
    v->display();

    cout << "\n-- Bike Info --\n";
    Bike bike;
    v = &bike;
    v->input();
    v->display();

    cout << "\n-- Truck Info --\n";
    Truck truck;
    v = &truck;
    v->input();
    v->display();

    return 0;
}