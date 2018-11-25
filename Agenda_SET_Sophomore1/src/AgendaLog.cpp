//
//  AgendaLog.cpp
//  Agenda
//
//  Created by 许滨楠 on 2018/11/23.
//  Copyright © 2018 Pie. All rights reserved.
//

#include "AgendaLog.hpp"
#include "Date.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
#include <time.h>

using std::string;
using std::setw;
using std::endl;

int AgendaLog::fileCount = 1;

string toString(int n) {
    string res = "";
    std::stringstream ss;
    ss << n;
    ss >> res;
    return res;
}

string AgendaLog::getFileName() {
    string fileName = "./log/AgendaLog-";
    fileName += toString(fileCount);
    fileName += ".log";
    std::ofstream logFile(fileName, std::ios::app);
    logFile.seekp(0, std::ios::end);
    std::streampos flag = logFile.tellp();
    if (flag >= 1024) {
        ++fileCount;
        logFile.close();
        return getFileName();
    }
    logFile.close();
    return fileName;
}

AgendaLog::AgendaLog() {
    
}

void AgendaLog::writeLog(string user, string operation, string object, string status) {
    std::ofstream logFile(getFileName(), std::ios::app);
    logFile.seekp(0, std::ios::end);
    std::streampos flag = logFile.tellp();
    if (flag == 0) {
        logFile << std::setiosflags(std::ios::left)
        << setw(18) << "[time]"
        << setw(18) << "[user]"
        << setw(32) << "[operation]"
        << setw(32) << "[object]"
        << "[status]" << endl;
    }
    time_t curTime = time(NULL);
    struct tm * theTime = localtime(&curTime);
    Date nowDate(theTime->tm_year + 1990, theTime->tm_mon, theTime->tm_mday,
                 theTime->tm_hour, theTime->tm_min);
    logFile << std::setiosflags(std::ios::left)
            << Date::dateToString(nowDate) << "  "
            << setw(18) << user
            << setw(32) << operation
            << setw(32) << object
            << status << endl;
    logFile.close();
}
