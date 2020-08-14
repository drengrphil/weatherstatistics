/*
 * Phillip Oni
 * 
 */

#ifndef WEATHERSTATISTICS_H_
#define WEATHERSTATISTICS_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <ctime>

using namespace std;

#define DATASOURCE_PATH "sourcedata/"

class WeatherStatistics
{
public:
    WeatherStatistics();
    bool isValidDateTime(string date, string time);
    double calculateCoefficient(string startDate, string startTime, string endDate, string endTime);

private:
    map<time_t, double> m_timePressure;
    void loadWeatherData();
    time_t getStartDateTime();
    time_t getEndDateTime();
    time_t dateTimeConverter(string date, string time);
};

WeatherStatistics::WeatherStatistics()
{
    // Load weather data from datasource.
    loadWeatherData();
}

/*
 * Checks for validate input date and time.
 */
inline bool WeatherStatistics::isValidDateTime(string date, string time){
    time_t dateTimeStamp = dateTimeConverter(date, time);

    if (dateTimeStamp < 0){
        return false;
    }

    stringstream strm;
    strm << dateTimeStamp;
    cout << "User Date Time: " << strm.str() << endl;

    if ( (dateTimeStamp < getStartDateTime()) || (dateTimeStamp > getEndDateTime()) ){
        cerr << "ERROR: " << date << "-" << time << " is out of range" << endl;
        return false;
    }
    return true;
}

/*
 * Calculate pressure coefficient
 */
inline double WeatherStatistics::calculateCoefficient(
                                    string startDate, 
                                    string startTime, 
                                    string endDate, 
                                    string endTime
                                    )
{
    time_t startDateTime = dateTimeConverter(startDate, startTime);
    time_t endDateTime = dateTimeConverter(endDate, endTime);

    if (endDateTime < startDateTime){
        cerr << "ERROR: The start date must be before the end date" << endl;
        exit(1);
    }

    // Locate appropriate data based on input date/time range
    map<time_t, double>::iterator iterStartTimePressure = m_timePressure.lower_bound(startDateTime);
    map<time_t, double>::iterator iterEndTimePressure = m_timePressure.lower_bound(endDateTime);

    // Time within the start and end range.
    double timeRangeDiff = iterEndTimePressure->first - iterStartTimePressure->first;
    // Pressure range difference
    double pressureRangeDiff = iterEndTimePressure->second - iterStartTimePressure->second;

    // Return slope
    return (pressureRangeDiff / timeRangeDiff);
}

/*
 * Load weather data from source path and store in m_timePressure.
 */
inline void WeatherStatistics::loadWeatherData(){
    // Load corresponding data for each year.
    for (int yr = 2012; yr <= 2015; yr++){
        ostringstream fileStream;
        fileStream << DATASOURCE_PATH << "Environmental_Data_Deep_Moor_"<<yr<<".txt";
        string dataFileName = fileStream.str();
        cout << "Loading " << dataFileName << endl;

        fstream dataFileStream;
        dataFileStream.open(dataFileName);

        string fileLine;
        // Skip header line
        getline(dataFileStream, fileLine);
        int con = 0;
        // Start processing data lines
        while ( getline(dataFileStream, fileLine) ){
            string date, time;
            // Data fields
            double airTemp, barometricPressure, dewPoint, 
                relativeHumidity, windDir, windGust, windSpeed;
            istringstream dataBuffer(fileLine);

            dataBuffer >> date >> time >> airTemp >> barometricPressure >> dewPoint >> relativeHumidity
                >> windDir >> windGust >> windSpeed;
            time_t dateTime = dateTimeConverter(date, time);
            m_timePressure[dateTime] = barometricPressure;
        }
        dataFileStream.close();
    }
}

inline time_t WeatherStatistics::getStartDateTime(){
    time_t timeVal = m_timePressure.begin()->first;
    stringstream str;
    str << timeVal;
    cout << "Start Date from Map: " << timeVal << " / " << str.str() <<endl;
    return timeVal;
}

inline time_t WeatherStatistics::getEndDateTime(){
    time_t timeVal = m_timePressure.rbegin()->first;
    stringstream str;
    str << timeVal;
    cout << "End Date from Map: " << timeVal << " / " << str.str() <<endl;
    return timeVal;
}

/*
 * Date and time conversion.
 */
inline time_t WeatherStatistics::dateTimeConverter(string date, string time){
    int yy, mon, dd = 0;
    // Parse date string.
    if (sscanf(date.c_str(), "%d_%d_%d", &yy, &mon, &dd) != 3){
        cerr << "ERROR: Failed to parse date " << date << endl;
        return -1;
    }

    // Parse time
    int hh, mm, ss = 0;
    if (sscanf(time.c_str(), "%d:%d:%d", &hh, &mm, &ss) != 3){
        cerr << "ERROR: Failed to parse time " << date << endl;
        return -2;
    }

    struct tm dateTimeVal = {};
    dateTimeVal.tm_year = yy - 1900; // Years since 1900
    dateTimeVal.tm_mon = mon - 1;    // Months since January
    dateTimeVal.tm_mday = dd;        // Day of the month
    dateTimeVal.tm_hour = hh;        // Hours of the day
    dateTimeVal.tm_min = mm;         // Minutes after the hour
    dateTimeVal.tm_sec = ss;         // Seconds after the minute.

    // Time value in seconds
    return mktime(&dateTimeVal);
}

#endif // WEATHERSTATISTICS_H_