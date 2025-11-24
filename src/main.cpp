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
    //cout << "attempting to load data from " << filename << endl; //debugging line 
    vector<Car> cars;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "ERROR: Cannot open " << filename << endl;
        return cars;
    }

    string line;
    getline(file, line); // skipping header

    //cout << "loading data..." << endl; //debugging line

    while (getline(file, line)) {
        stringstream ss(line);
        Car car;
        string price_str, year_str, mileage_str, skip;

        getline(ss, skip, ',');      // ID / index column skipped
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
    //cout << "data loading complete. " << cars.size() << " records loaded." << endl; //debugging line
    return cars;
}

int main(int argc, char* argv[]) {
    //cout << "Program started" << endl; //debugging line
    if (argc < 5) {
        cout << "Usage: car_search <hashtable|treemap> <brand> <model> <year>" << endl;
        return 1;
    }

    string structure = argv[1];
    string brand = argv[2];
    string model = argv[3];
    int year = stoi(argv[4]);

    // Load data w/ checks - updated to include error handling
    vector<Car> allCars = loadCarsFromCSV("data.csv"); //Updated - issue with accessing file

    if (allCars.empty()) {
        cout << "ERROR: No data loaded" << endl;
        return 1;
    }

    // search key - refer to car.h to ensure format matches
    string searchKey = brand + "|" + model + "|" + to_string(year);
    vector<Car> results;

    auto start = chrono::high_resolution_clock::now();

    if (structure == "hashtable") {
        //cout << "running hashtable" << endl; //debugging line
        HashTable<string, Car> ht;
        for (const auto& car : allCars)
            ht.insert(car.getKey(), car);
        //cout << "hashtable complete" << endl; //debugging line
        results = ht.search(searchKey);
    }
    else {
        TreeMap<string, Car> tm;
        for (const auto& car : allCars)
            tm.insert(car.getKey(), car);
        results = tm.search(searchKey);
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    // Output results
    cout << "TIME_MS:" << duration.count() / 1000.0 << endl;
    cout << "COUNT:" << results.size() << endl;

    if (!results.empty()) {
        // ----- price stats -----
        double lowest  = results[0].price;
        double highest = results[0].price;
        double sum     = 0.0;

        const Car* lowestCar  = &results[0];
        const Car* highestCar = &results[0];

        for (const auto& car : results) {
            if (car.price < lowest) {
                lowest = car.price;
                lowestCar = &car;
            }
            if (car.price > highest) {
                highest = car.price;
                highestCar = &car;
            }
            sum += car.price;
        }

        // median price + median example
        vector<Car> sorted = results;
        sort(sorted.begin(), sorted.end(),
             [](const Car& a, const Car& b) { return a.price < b.price; });

        Car medianCar;
        size_t n = sorted.size();
        if (n % 2 == 1)
            medianCar = sorted[n / 2];
        else
            medianCar = sorted[n / 2];

        double medianPrice = medianCar.price;

        cout << "LOWEST:" << lowest << endl;
        cout << "HIGHEST:" << highest << endl;
        cout << "AVERAGE:" << (sum / results.size()) << endl;
        cout << "MEDIAN:" << medianPrice << endl;

        cout << "---CARS---" << endl;

        // Example matches for lowest / median / highest
        cout << "Lowest example: " << lowestCar->toString() << endl;
        cout << "Median example: " << medianCar.toString() << endl;
        cout << "Highest example: " << highestCar->toString() << endl;
        cout << endl;

        // All matching vehicles
        for (const auto& car : results)
            cout << car.toString() << endl;
    }
    else
        cout << "No matching cars found." << endl;

    return 0;
}
