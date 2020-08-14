/*
 * Main for the weather statistics
 * @Author: Phillip O.
 */
#include <iostream>
#include <string>
#include "weatherstatistics.hpp"

using namespace std;

int main(){
    WeatherStatistics weatherStats;

    // User inputs
    string startDate, startTime;
    cout << endl << "Enter Start Date (yyyy_mm_dd): ";
    cin >> startDate;
    cout << "Enter Start Time (hh:mm:ss): ";
    cin >> startTime;

    if (!weatherStats.isValidDateTime(startDate, startTime)){
        return 1;
    }

    string endDate, endTime;
    cout << "Enter End Date (yyyy_mm_ddd): ";
    cin >> endDate;
    cout << "Enter End Time (hh:mm:ss): ";
    cin >> endTime;

    if (!weatherStats.isValidDateTime(endDate, endTime)){
        return 1;
    }

    double coefficientVal = weatherStats.calculateCoefficient(startDate, startTime, endDate, endTime);
    cout << endl << "Estimated Coefficient: " << coefficientVal * 24 * 60 * 60 << " inHg/day" << endl;

    if (coefficientVal < 0){
        cout << "Stormy Weather" << endl;
    } else {
        cout << "Sunny Weather" << endl;
    }

    return 0;
}