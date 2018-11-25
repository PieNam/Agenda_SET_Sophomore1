//
//  Date.cpp
//  Agenda
//
//  Created by 许滨楠 on 2018/11/10.
//  Copyright © 2018 Pie. All rights reserved.
//

#include "Date.hpp"

#include <string>
#include <sstream>
#include <stdio.h>
#include <regex>

//own function
int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int isLeap(int year) {
    return ((year % 4 == 0 && year % 100 != 0)||(year % 400 == 0));
}
int maxDay(int year, int month) {
    if (year < 1000 || year > 9999 || month < 1 || month > 12) return 0;
    if (isLeap(year) && month == 2) return 29;
    else return days[month - 1];
}
int pow(int b, int p) {
    if (!p) return 1;
    int res = b;
    for (int i = 1; i < p; ++i) {
        res *= b;
    }
    return res;
}
bool isNum(char t) {
    return (t >= '0' && t <= '9');
}
int strToInt(std::string s) {
    int res;
    std::stringstream ss;
    ss << s;
    ss >> res;
    return res;
}


Date::Date() {
    m_year = 0;
    m_month = 0;
    m_day = 0;
    m_hour = 0;
    m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
    m_year = t_year;
    m_month = t_month;
    m_day = t_day;
    m_hour = t_hour;
    m_minute = t_minute;
}

Date::Date(const std::string &dateString) {
    *this = stringToDate(dateString);
}

int Date::getYear(void) const {
    return m_year;
}

void Date::setYear(const int t_year) {
    m_year = t_year;
}

int Date::getMonth(void) const {
    return m_month;
}

void Date::setMonth(const int t_month) {
    m_month = t_month;
}

int Date::getDay(void) const {
    return m_day;
}

void Date::setDay(const int t_day) {
    m_day = t_day;
}

int Date::getHour(void) const {
    return m_hour;
}

void Date::setHour(const int t_hour) {
    m_hour = t_hour;
}

int Date::getMinute(void) const {
    return m_minute;
}

void Date::setMinute(const int t_minute) {
    m_minute = t_minute;
}

bool Date::isValid(const Date &t_date) {
    int y = t_date.m_year;
    int m = t_date.m_month;
    int d = t_date.m_day;
    int h = t_date.m_hour;
    int min = t_date.m_minute;
    return (y >= 1000 && y <= 9999 && m > 0 && m <= 12 &&
            d > 0 && d <= maxDay(y, m) &&
            h >= 0 && h < 24 && min >= 0 && min < 60);
}

Date Date::stringToDate(const std::string &t_dateString) {
    std::regex dateForm("[0-9]{4}-[0-9]{2}-[0-9]{2}/[0-9]{2}:[0-9]{2}");
    if (!std::regex_match(t_dateString, dateForm)) return Date();
    int y = strToInt(t_dateString.substr(0, 4));
    int m = strToInt(t_dateString.substr(5, 2));
    int d = strToInt(t_dateString.substr(8, 2));
    int h = strToInt(t_dateString.substr(11, 2));
    int min = strToInt(t_dateString.substr(14, 2));
    return Date(y, m, d, h, min);
}

std::string Date::dateToString(const Date &t_date) {
    int y = 0;
    int m = 0;
    int d = 0;
    int h = 0;
    int min = 0;
    if (isValid(t_date)) {
        y = t_date.m_year;
        m = t_date.m_month;
        d = t_date.m_day;
        h = t_date.m_hour;
        min = t_date.m_minute;
    }
    char res[16];
    sprintf(res, "%04d-%02d-%02d%c%02d:%02d", y, m, d, '/', h, min);
    std::string result = res;
    return result;
}

Date& Date::operator=(const Date &t_date) {
    m_year = t_date.m_year;
    m_month = t_date.m_month;
    m_day = t_date.m_day;
    m_hour = t_date.m_hour;
    m_minute = t_date.m_minute;
    return *this;
}

bool Date::operator==(const Date &t_date) const {
    return (t_date.m_year == m_year && t_date.m_month == m_month &&
            t_date.m_day == m_day && t_date.m_hour == m_hour &&
            t_date.m_minute == m_minute);
}

bool Date::operator>(const Date &t_date) const {
    std::string th = dateToString(*this);
    std::string ot = dateToString(t_date);
    for (int i = 0; i < 16; ++i) {
        if (th[i] > ot[i]) return true;
        if (th[i] < ot[i]) return false;
    }
    return false;
}

bool Date::operator<(const Date &t_date) const {
    std::string th = dateToString(*this);
    std::string ot = dateToString(t_date);
    for (int i = 0; i < 16; ++i) {
        if (th[i] < ot[i]) return true;
        if (th[i] > ot[i]) return false;
    }
    return false;
}

bool Date::operator>=(const Date &t_date) const {
    return !(*this < t_date);
}

bool Date::operator<=(const Date &t_date) const {
    return !(*this > t_date);
}

