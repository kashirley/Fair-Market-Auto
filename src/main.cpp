#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include "car.h"
#include "hashtable.h"
#include "treemap.h"

using namespace std;

//CSV LOADING 
vector<Car> loadCarsFromCSV(const string& filename) {
    vector<Car> cars;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "ERROR: Cannot open " << filename << endl;
        return cars;
    }

    string line;
    getline(file, line);  

    while (getline(file, line)) {
        stringstream ss(line);
        Car car;
        string price_str, year_str, mileage_str, skip;

        getline(ss, skip, ',');           
        getline(ss, car.brand, ',');
        getline(ss, car.model, ',');
        getline(ss, year_str, ',');
        getline(ss, car.color, ',');
        getline(ss, mileage_str, ',');
        getline(ss, price_str, ',');
        getline(ss, car.condition, ',');

        try {
            car.year    = stoi(year_str);
            car.mileage = stoi(mileage_str);
            car.price   = stod(price_str);
        } catch (...) {
            continue;
        }

        cars.push_back(car);
    }

    file.close();
    return cars;
}

//HELPERS 
double mean(const vector<double>& prices) {
    if (prices.empty()) return 0.0;
    return accumulate(prices.begin(), prices.end(), 0.0) / prices.size();
}

double median(vector<double> prices) {
    if (prices.empty()) return 0.0;
    sort(prices.begin(), prices.end());
    size_t n = prices.size();
    if (n % 2 == 1) return prices[n / 2];
    return (prices[n / 2 - 1] + prices[n / 2]) / 2.0;
}

// MAIN 
int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Usage: main.exe <hashtable|treemap> <brand> <model> <year>\n";
        return 1;
    }

    string structure = argv[1];
    string brand     = argv[2];
    string model     = argv[3];
    int year         = 0;

    try {
        year = stoi(argv[4]);
    } catch (...) {
        cerr << "ERROR: Year must be an integer.\n";
        return 1;
    }

    vector<Car> allCars = loadCarsFromCSV("data.csv");
    if (allCars.empty()) {
        cerr << "ERROR: No data loaded from data.csv\n";
        return 1;
    }

    string searchKey = brand + "|" + model + "|" + to_string(year);
    vector<Car> results;

    // Build + search chosen data structure (timed) 
    auto start = chrono::high_resolution_clock::now();

    if (structure == "hashtable") {
        HashTable<string, Car> ht;
        for (const auto& car : allCars)
            ht.insert(car.getKey(), car);

        results = ht.search(searchKey);
    }
    else if (structure == "treemap") {
        TreeMap<string, Car> tm;
        for (const auto& car : allCars)
            tm.insert(car.getKey(), car);

        results = tm.search(searchKey);
    }
    else {
        cerr << "ERROR: structure must be 'hashtable' or 'treemap'\n";
        return 1;
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    //Output for GUI 
    cout << "TIME_MS: " << duration_ms << "\n";
    cout << "COUNT: " << results.size() << "\n";

    if (results.empty()) {
        cout << "LOWEST: 0\n";
        cout << "HIGHEST: 0\n";
        cout << "MEDIAN: 0\n";
        cout << "AVERAGE: 0\n";
        cout << "---CARS---\n";
        return 0;
    }

    // Collect prices
    vector<double> prices;
    prices.reserve(results.size());
    for (const auto& c : results)
        prices.push_back(c.price);

    double low  = *min_element(prices.begin(), prices.end());
    double high = *max_element(prices.begin(), prices.end());
    double avg  = mean(prices);
    double med  = median(prices);

    cout << "LOWEST: "  << low  << "\n";
    cout << "HIGHEST: " << high << "\n";
    cout << "MEDIAN: "  << med  << "\n";
    cout << "AVERAGE: " << avg  << "\n";

    // Print all cars in ascending price order 
    vector<Car> sortedResults = results;
    sort(sortedResults.begin(), sortedResults.end(),
         [](const Car& a, const Car& b) {
             return a.price < b.price;
         });

    cout << "---CARS---\n";
    for (const auto& car : sortedResults) {
        cout << car.toString() << "\n";
    }

    return 0;
}
