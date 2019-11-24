#ifndef DATETIME_H
#define DATETIME_H

#include <cstdio>
#include <sstream>

#include "IJsonHandle.h"

using namespace std;

enum class DateMode {
    DDMMYYYY,
    YYYYMMDD,
    MMDDYYYY,
    NUM_OF_DATE_MODE
};

enum class TimeMode {
    HHMMSS,
    HHMM,
    NUM_OF_TIME_MODE
};

/** Date structure
    day: 1 <= day <= 31
    month: 1 <= month <= 12
    year: -INT_MAX <= year <= INT_MAX
**/

struct TDate {
    static DateMode dateMode;
    static int daysInMonth[12];
    int year, month, day;
    TDate() {}
    TDate(int year, int month, int day): year(year), month(month), day(day) {}

    bool isValid() const // check if (day,month,year) is a valid date
    {
        return (1 <= day && day <= 31 && 1 <= month && month <= 12);
    }
    friend ostream& operator<<(ostream &os, const TDate& date)
    {

        char buffer[] = "does not recognize this date mode";
        switch (TDate::dateMode)
        {
        case DateMode::DDMMYYYY:
            sprintf(buffer,"%02d/%02d/%04d",date.day,date.month,date.year);
            break;
        case DateMode::YYYYMMDD:
            sprintf(buffer,"%04d/%02d/%02d",date.year,date.month,date.day);
            break;
        default:
            //buffer = "does not recognize this date mode";
            break;
        }
        string s(buffer);
        os << s;
        if (!date.isValid())
        {
            os << " is not a valid day !";
        }
        return os;
    }

    // return the day different between 2 TDate
    int operator-(const TDate& that)
    {
        // doesn't count leap year
        int yearDiff = year - that.year;

        if (yearDiff == 0)
        {
            int days = 0;
            for(int i = that.month; i < month; i++) days += daysInMonth[i-1];
            days += day;
            days -= that.day;
            return days;
        }
        else // will add later
        {

        }
        return -9999;
    }
};

// default Date mode is YYYY/MM/DD
DateMode TDate::dateMode = DateMode::YYYYMMDD;
// the number of day in a specific month
int TDate::daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
    Time Structure (24 hours clock)
    0 <= hour < 24
    0 <= minute < 60
    0 <= seconds < 60
**/

struct TTime {
    static TimeMode timeMode;
    int hour, minute, second;
    TTime() {}
    TTime(int hour, int minute=0, int second=0):
        hour(hour), minute(minute), second(second) {}

    bool isValid() const {return 0 <= hour && hour < 24
                                && 0 <= minute && minute < 60
                                && 0 <= second && second < 60;}

    friend ostream& operator<<(ostream &os, const TTime& a)
    {


        char buffer[] = "does not recognize this time mode";
        switch (TTime::timeMode)
        {
        case TimeMode::HHMMSS:
            sprintf(buffer,"%02d:%02d:%02d",a.hour, a.minute, a.second);
            break;
        case TimeMode::HHMM:
            sprintf(buffer,"%02d:%02d",a.hour, a.minute);
            break;
        default:
            //buffer = "does not recognize this date mode";
            break;
        }
        string s(buffer);
        os << s;
        if (!a.isValid())
        {
            os << " is not a valid time !";
        }
        return os;
    }
};
TimeMode TTime::timeMode = TimeMode::HHMMSS; // default Time mode is HH:MM:SS

struct TDateTime {
    TDate date;
    TTime time;
    TDateTime(): date(0,0,0), time(0,0,0) {}
    TDateTime(int year, int month, int day,
                int hour, int minute, int second):
        date(year, month, day), time(hour, minute, second) {}
    TDateTime(TDate date, TTime time): date(date), time(time) {}
    TDateTime(TTime time, TDate date): date(date), time(time) {}
    TDateTime(TTime time): date(0,0,0), time(time) {}
    TDateTime(TDate date): date(date), time(0,0,0) {}
    TDateTime(std::string isoFormat)
    {
        std::stringstream ss(isoFormat);
        char tmp;
        ss >> date.year >> tmp;
        ss >> date.month >> tmp;
        ss >> date.day >> tmp;
        ss >> time.hour >> tmp;
        ss >> time.minute >> tmp;
        ss >> time.second >> tmp;
    }
    friend ostream& operator<<(ostream &os, const TDateTime& a)
    {
        os << a.date << " "  << a.time;
        return os;
    }
    /**
        date time will follow ISO 8601 format: YYYY-MM-DDThh:mm:ssZ
    */
    std::string toJson()
    {
        char buffer[] = "YYYY-MM-DDThh:mm:ssZ";
        sprintf(buffer, "%04d-%02d-%02dT%02d:%02d:%02dZ",
                date.year, date.month, date.day,
                time.hour, time.minute, time.second);
        std::string s(buffer);
        return s;
    }

    TDateTime& operator=(std::string isoFormat)
    {
        std::stringstream ss(isoFormat);
        char tmp;
        ss >> date.year >> tmp;
        ss >> date.month >> tmp;
        ss >> date.day >> tmp;
        ss >> time.hour >> tmp;
        ss >> time.minute >> tmp;
        ss >> time.second >> tmp;
        return *this;
    }
};

#endif // DATETIME_H

