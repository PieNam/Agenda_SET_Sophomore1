//
//  main.cpp
//  Agenda
//
//  Created by 许滨楠 on 2018/11/10.
//  Copyright © 2018 Pie. All rights reserved.
//

#include "AgendaLog.hpp"
#include "AgendaService.hpp"
#include "AgendaUI.hpp"
#include "Date.hpp"
#include "Meeting.hpp"
#include "Storage.hpp"
#include "User.hpp"

#include <iostream>
#include <string>
#include <vector>

#include <csignal>

using namespace std;

AgendaUI aui;

void signalHandler(int sigInt) {
    cout << endl << "[System] System has received interrupt signal from user." << endl
         << "[System] terminating agenda system... your data would be saved." << endl;
    aui.signalQuit();
    exit(1);
}

int main() {
    signal(SIGINT, signalHandler);
    
    aui.OperationLoop();
    AgendaLog test;
    return 0;
}
