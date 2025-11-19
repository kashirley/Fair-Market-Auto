#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include "car.h"
#include "hashtable.h"
#include "treemap.h"

using namespace std;

vector<Car> loadCarsFromCSV(const string& filename) {
    vector<Car> cars;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "ERROR: Cannot open " << filename << endl;
        return cars;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        Car car;
        string price_str, year_str, mileage_str;

        getline(ss, car.brand, ',');
        getline(ss, car.model, ',');
        getline(ss, year_str, ',');
        getline(ss, car.color, ',');
        getline(ss, mileage_str, ',');
        getline(ss, price_str, ',');
        getline(ss, car.condition, ',');

        car.year = stoi(year_str);
        car.mileage = stoi(mileage_str);
        car.price = stod(price_str);

        cars.push_back(car);
    }

    file.close();
    return cars;
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        //cout << "Usage: car_search <hashtable|treemap> <brand> <model> <year>" << endl;
        return 1;
    }

    string structure = argv[1];
    string brand = argv[2];
    string model = argv[3];
    int year = stoi(argv[4]);

	// Load data w/ checks - updated to include error handling
    vector<Car> allCars = loadCarsFromCSV("data/car_data.csv");

    if (allCars.empty()) {
        cout << "ERROR: No data loaded" << endl;
        return 1;
    }
	//search key - delimeter tested with '|'
    string searchKey = brand + "|" + model + "|" + to_string(year);
    vector<Car> results;

    auto start = chrono::high_resolution_clock::now();

    if (structure == "hashtable") {
        HashTable<string, Car> ht;
        for (const auto& car : allCars) {
            ht.insert(car.getKey(), car);
        }
        results = ht.search(searchKey);
    }
    else {
        TreeMap<string, Car> tm;
        for (const auto& car : allCars) {
            tm.insert(car.getKey(), car);
        }
        results = tm.search(searchKey);
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    // Output results
    cout << "TIME_MS:" << duration.count() / 1000.0 << endl;
    cout << "COUNT:" << results.size() << endl;

    if (!results.empty()) {
        double lowest = results[0].price;
        double highest = results[0].price;
        double sum = 0;

        for (const auto& car : results) {
            lowest = min(lowest, car.price);
            highest = max(highest, car.price);
            sum += car.price;
        }

        cout << "LOWEST:" << lowest << endl;
        cout << "HIGHEST:" << highest << endl;
        cout << "AVERAGE:" << (sum / results.size()) << endl;
        cout << "---CARS---" << endl;

        for (const auto& car : results) {
            cout << car.toString() << endl;
        }
    }
    else {
        cout << "No matching cars found." << endl;
    }

    return 0;
}