//
//  AgendaLog.hpp
//  Agenda
//
//  Created by 许滨楠 on 2018/11/23.
//  Copyright © 2018 Pie. All rights reserved.
//

#ifndef AgendaLog_h
#define AgendaLog_h

#include <iostream>
#include <string>

using std::string;

class AgendaLog {
private:
    static int fileCount;

public:
    AgendaLog();
    
    string getFileName();
    
    void writeLog(string, string, string, string);
};

#endif /* AgendaLog_h */
