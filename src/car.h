#ifndef CAR_H
#define CAR_H
#include <string>
#include <sstream>
using namespace std;

struct Car {
    string brand;
    string model;
    int year;
    string color;
    int mileage;
    double price;
    string condition;

    // Composite key generation (not used in map-only version but kept for compatibility)
    string getKey() const {
        return brand + "|" + model + "|" + to_string(year);
    }

    // Convert to readable string
    string toString() const {
        ostringstream oss;
        oss << year << " " << brand << " " << model
            << " (" << color << ", " << condition << ") - $"
            << price << " - " << mileage << " mi";
        return oss.str();
    }
}; 
#endif
