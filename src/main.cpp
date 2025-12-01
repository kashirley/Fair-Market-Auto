#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <chrono>
#include "car.h"

using namespace std;


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

void printStats(const vector<Car>& results) {
    if (results.empty()) {
        cout << "No cars found.\n";
        return;
    }

    vector<double> prices;
    for (const auto& c : results) prices.push_back(c.price);

    double low = *min_element(prices.begin(), prices.end());
    double high = *max_element(prices.begin(), prices.end());
    double avg = mean(prices);
    double med = median(prices);

    cout << "Matches: " << results.size() << "\n";
    cout << "Lowest price:  $" << low << "\n";
    cout << "Highest price: $" << high << "\n";
    cout << "Mean price:    $" << avg << "\n";
    cout << "Median price:  $" << med << "\n\n";
}



using CarMap = map<int, Car>;

vector<Car> searchByMake(const CarMap& carMap, const string& make) {
    vector<Car> results;
    for (const auto& [id, car] : carMap)
        if (car.brand == make) results.push_back(car);
    return results;
}

vector<Car> searchByModel(const CarMap& carMap, const string& model) {
    vector<Car> results;
    for (const auto& [id, car] : carMap)
        if (car.model == model) results.push_back(car);
    return results;
}

vector<Car> searchByCondition(const CarMap& carMap, const string& cond) {
    vector<Car> results;
    for (const auto& [id, car] : carMap)
        if (car.condition == cond) results.push_back(car);
    return results;
}



int main() {
    cout << "=== Car Inventory Program (std::map Version) ===\n";

    // 1. Load cars
    vector<Car> allCars = loadCarsFromCSV("data.csv");

    if (allCars.empty()) {
        cerr << "No data loaded. Make sure data.csv exists.\n";
        return 1;
    }

    cout << "Loaded " << allCars.size() << " cars.\n";

    // 2. Insert into map (timed)
    CarMap carMap;
    auto startInsert = chrono::high_resolution_clock::now();

    int id = 0;
    for (const auto& car : allCars)
        carMap[id++] = car;

    auto endInsert = chrono::high_resolution_clock::now();
    auto insertTime = chrono::duration_cast<chrono::milliseconds>(endInsert - startInsert);

    cout << "Insertion time: " << insertTime.count() << " ms\n\n";

    // 3. Search loop
    while (true) {
        cout << "Search Options:\n";
        cout << "  1 = Make\n";
        cout << "  2 = Model\n";
        cout << "  3 = Condition\n";
        cout << "  0 = Exit\n";
        cout << "Choose: ";

        int choice;
        cin >> choice;
        if (choice == 0) break;

        string term;
        cout << "Enter search term: ";
        cin >> term;

        auto startSearch = chrono::high_resolution_clock::now();
        vector<Car> matches;

        if (choice == 1)
            matches = searchByMake(carMap, term);
        else if (choice == 2)
            matches = searchByModel(carMap, term);
        else if (choice == 3)
            matches = searchByCondition(carMap, term);
        else {
            cout << "Invalid selection.\n\n";
            continue;
        }

        auto endSearch = chrono::high_resolution_clock::now();
        auto searchTime = chrono::duration_cast<chrono::microseconds>(endSearch - startSearch);

        cout << "\nSearch time: " << searchTime.count() << " microseconds\n";
        printStats(matches);

        if (!matches.empty()) {
            cout << "Example cars:\n";
            int displayed = 0;
            for (const auto& car : matches) {
                cout << "  - " << car.toString() << "\n";
                if (++displayed >= 5) break;   // only first 5
            }
        }

        cout << "\n-------------------------------------------\n\n";
    }

    cout << "Goodbye!\n";
    return 0;
}
