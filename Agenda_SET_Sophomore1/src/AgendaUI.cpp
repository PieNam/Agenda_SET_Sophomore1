//
//  AgendaUI.cpp
//  Agenda
//
//  Created by 许滨楠 on 2018/11/15.
//  Copyright © 2018 Pie. All rights reserved.
//

#include "AgendaUI.hpp"
#include "AgendaService.hpp"
#include "AgendaLog.hpp"
#include "Date.hpp"
#include "Meeting.hpp"
#include "Storage.hpp"
#include "User.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <list>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::setw;
using std::string;
using std::ios;
using std::setiosflags;
using std::list;
using std::vector;

AgendaUI::AgendaUI() {
    m_userName = "";
    m_userPassword = "";
    startAgenda();
}

void AgendaUI::signalQuit() {
    quitAgenda();
}

void AgendaUI::OperationLoop() {
    while (true) {
        for (int i = 0; i < 26; ++i) cout << '-';
        cout << " Agenda ";
        for (int i = 0; i < 26; ++i) cout << '-';
        cout << endl;
        cout << "Action :" << endl;
        if (m_userName == "") {
            cout << "l    - log in Agenda by user name and password" << endl;
            cout << "r    - register an Agenda account" << endl;
            cout << "q    - quit Agenda" << endl;
        }
        else {
            cout << "o    - log out Agenda" << endl;
            cout << "dc   - delete Agenda account" << endl;
            cout << "lu   - list all Agenda user" << endl;
            cout << "cm   - create a meeting" << endl;
            cout << "amp  - add meeting participator" << endl;
            cout << "rmp  - remove meeting participator" << endl;
            cout << "rqm  - quit meeting" << endl;
            cout << "la   - list all meetings" << endl;
            cout << "las  - list all sponsor meetings" << endl;
            cout << "lap  - list all participator meetings" << endl;
            cout << "qm   - query meeting by title" << endl;
            cout << "qt   - query meeting by time interval" << endl;
            cout << "dm   - delete meeting by title" << endl;
            cout << "da   - delete all meetings" << endl;
        }
        for (int i = 1; i <= 60; i++) cout << "-";
        cout << endl;
        cout << endl;
        if (m_userName == "") cout << "Agenda :~$ ";
        else cout << "Agenda@" << m_userName << " :~# ";
        if (!executeOperation(getOperation())) break;
    }
}

void AgendaUI::startAgenda() {
    system("clear");
}

string AgendaUI::getOperation() {
    string opr;
    cin >> opr;
    return opr;
}

bool AgendaUI::executeOperation(string opr) {
    for (int i = 0; i < opr.size(); ++i)
        if (opr[i] >= 'A' && opr[i] <= 'Z')
            opr[i] = opr[i] - 'A' + 'a';
    
    if (m_userName == "") {
        if (opr == "l") {
            userLogIn();
            return true;
        }
        if (opr == "r") {
            userRegister();
            return true;
        }
        if (opr == "q") {
            m_agendaService.quitAgenda();
            return false;
        }
    }
    else {
        if (opr == "o") {
            userLogOut();
            return true;
        }
        if (opr == "dc") {
            deleteUser();
            return true;
        }
        if (opr == "lu") {
            listAllUsers();
            return true;
        }
        if (opr == "cm") {
            createMeeting();
            return true;
        }
        if (opr == "amp") {
            addMeetingParticipator();
            return true;
        }
        if (opr == "rmp") {
            removeMeetingParticipator();
            return true;
        }
        if (opr == "rqm") {
            quitMeeting();
            return true;
        }
        if (opr == "la") {
            listAllMeetings();
            return true;
        }
        if (opr == "las") {
            listAllSponsorMeetings();
            return true;
        }
        if (opr == "lap") {
            listAllParticipateMeetings();
            return true;
        }
        if (opr == "qm") {
            queryMeetingByTitle();
            return true;
        }
        if (opr == "qt") {
            queryMeetingByTimeInterval();
            return true;
        }
        if (opr == "dm") {
            deleteMeetingByTitle();
            return true;
        }
        if (opr == "da") {
            deleteAllMeetings();
            return true;
        }
    }
    cout << endl << "Agenda : [" << opr << "] : command not found" << endl << endl;
    return true;
}

void AgendaUI::userLogIn() {
    cout << endl << "[log in] [user name] [password]" << endl;
    cout << "[log in] ";
    cin >> m_userName >> m_userPassword;
    if (m_agendaService.userLogIn(m_userName, m_userPassword)) {
        cout << "[log in] succeed!";
        m_log.writeLog(m_userName, "log in by name and password", m_userName, "succeed");
    }
    else {
        cout << "[log in] password error or user doesn't exit";
        m_userName = "";
        m_userPassword = "";
        m_log.writeLog(m_userName, "log in by name and password", m_userName, "fail");
    }
    cout << endl << endl;
}

void AgendaUI::userRegister(){
    cout << endl << "[register] [username] [password] [email] [phone]" << endl;
    cout << "[register] ";
    string newUserName, newPassword, newEmail, newPhone;
    cin >> newUserName >> newPassword >> newEmail >> newPhone;
    if (m_agendaService.userRegister(newUserName, newPassword, newEmail, newPhone)) {
        cout << "[register] succeed!";
        m_log.writeLog("system", "register user", newUserName, "succeed");
    }
    else {
        cout << "[register] This username has been registered!";
        m_log.writeLog("system", "register user", "none", "fail");
    }
    cout << endl << endl;
}

void AgendaUI::userLogOut(){
    m_log.writeLog(m_userName, "log out", "none", "succeed");
    m_userName = "";
    m_userPassword = "";
    cout << endl << endl;
}

void AgendaUI::quitAgenda() {
    m_log.writeLog("system", "quit agenda", "none", "succeed");
    m_agendaService.quitAgenda();
}

void AgendaUI::deleteUser(){
    m_agendaService.deleteUser(m_userName, m_userPassword);
    m_userName = "";
    m_userPassword = "";
    cout << endl << "[delete agenda account] succeed!" << endl << endl;
    m_log.writeLog(m_userName, "delete user", "deleted" + m_userName, "succeed");
}

void AgendaUI::listAllUsers(){
    cout << endl << "[list all users]" << endl << endl;
    cout << setiosflags(ios::left) << setw(8) << "name";
    cout << setw(16) << "email";
    cout << "phone" << endl;
    list<User> userList = m_agendaService.listAllUsers();
    for (auto i = userList.begin(); i != userList.end(); ++i) {
        cout << setw(8) << i->getName();
        cout << setw(16) << i->getEmail();
        cout << i->getPhone() << endl;
    }
    cout << endl;
    m_log.writeLog(m_userName, "query all users and list", "all users", "succeed");
}

string numberString(int n) {
    string res = "";
    std::stringstream s;
    s << n;
    s >> res;
    return res;
}

void AgendaUI::createMeeting(){
    cout << endl << "[create meeting] [the number of participators]" << endl;
    cout << "[create meeting] ";
    int n;
    cin >> n;
    if (n <= 0) {
        cout << "[create meeting] the number of participators must be positive" << endl;
        m_log.writeLog(m_userName, "create a meeting", "none", "fail");
        return;
    }
    vector<string> participators;
    string t;
    for (int i = 0; i < n; ++i) {
        cout << "[create meeting] [pleast enter the participator " << numberString(i + 1)
             << "]" << endl << "[create meeting] ";
        cin >> t;
        participators.push_back(t);
    }
    cout << "[create meeting] ";
    string title, startTime, endTime;
    cin >> title >> startTime >> endTime;
    if (m_agendaService.createMeeting(m_userName, title, startTime, endTime, participators)) {
        cout << "[create meeting] succeed!";
        m_log.writeLog(m_userName, "create a meeting", title, "succeed");
    }
    else {
        cout << "[create meeting] error!";
        m_log.writeLog(m_userName, "create a meeting", "none", "fail");
    }
    cout << endl << endl;
}

void AgendaUI::addMeetingParticipator() {
    cout << endl << "[add participator] [meeting title] [participator username]" << endl;
    cout << "[add participator] ";
    string title, participator;
    cin >> title >> participator;
    if (m_agendaService.addMeetingParticipator(m_userName, title, participator)) {
        cout << "[add participator] succeed!";
        m_log.writeLog(m_userName, "add meeting participator", participator + " to " + title, "succeed");
    }
    else {
        cout << "[add participator] error!";
        m_log.writeLog(m_userName, "add meeting participator", "none", "fail");
    }
    cout << endl << endl;
}

void AgendaUI::removeMeetingParticipator() {
    cout << endl << "[remove participator] [meeting title] [participator username]" << endl;
    cout << "[remove participator] ";
    string title, participator;
    cin >> title >> participator;
    if (m_agendaService.removeMeetingParticipator(m_userName, title, participator)) {
        cout << "[remove participator] succeed!";
        m_log.writeLog(m_userName, "remove meeting participator", participator + " from " + title, "succeed");
    }
    else {
        cout << "[remove participator] error!";
        m_log.writeLog(m_userName, "remove meeting participator", participator + " from " + title, "fail");
    }
    cout << endl << endl;
}

void AgendaUI::quitMeeting() {
    cout << endl << "[quit meeting] [meeting title]" << endl;
    cout << "[quit meeting] ";
    string title;
    cin >> title;
    if (m_agendaService.quitMeeting(m_userName, title)) {
        cout << "[quit meeting] succeed!";
        m_log.writeLog(m_userName, "quit meeting", "from " + title, "succeed");
    }
    else {
        cout << "[quit meeting] error!";
        m_log.writeLog(m_userName, "quit meeting", "from " + title, "fail");
    }
    cout << endl << endl;
}

void AgendaUI::listAllMeetings(){
    cout << endl << "[list all meetings]" << endl << endl;
    list<Meeting> meetingList = m_agendaService.listAllMeetings(m_userName);
    if(meetingList.empty()) {
        cout << "None" << endl << endl;
    }
    else printMeetings(meetingList);
    m_log.writeLog(m_userName, "list all meetings", "all meetings", "succeed");
}

void AgendaUI::listAllSponsorMeetings(){
    cout << endl << "[list all sponsor meetings]" << endl << endl;
    list<Meeting> meetingList = m_agendaService.listAllSponsorMeetings(m_userName);
    if (meetingList.empty()) {
        cout << "None" << endl << endl;
    }
    else {
        printMeetings(meetingList);
    }
    m_log.writeLog(m_userName, "list all sponsored meetings", "all sponsored meetings", "succeed");
}

void AgendaUI::listAllParticipateMeetings(){
    cout << endl << "[list all participator meetings]" << endl << endl;
    list<Meeting> meetingList = m_agendaService.listAllParticipateMeetings(m_userName);
    if (meetingList.empty()) {
        cout << "None" << endl << endl;
    }
    else {
        printMeetings(meetingList);
    }
    m_log.writeLog(m_userName, "list all participated meetings", "all participated meetings", "succeed");
}

void AgendaUI::queryMeetingByTitle(){
    cout << endl << "[query meeting] [title]" << endl;
    cout << "[query meeting] ";
    string title;
    cin >> title;
    list<Meeting> meetingList = m_agendaService.meetingQuery(m_userName, title);
    if (meetingList.empty()) {
        cout << "None" << endl << endl;
    }
    else {
        printMeetings(meetingList);
    }
    m_log.writeLog(m_userName, "query meeting by title", "corresponding meeting", "succeed");
}

void AgendaUI::queryMeetingByTimeInterval(){
    cout << endl << "[query meetings] [start time(yyyy-mm-dd/hh:mm)]"
         << "[end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meetings] ";
    string startTime, endTime;
    cin >> startTime >> endTime;
    cout << "[query meetings] " << endl;
    list<Meeting> meetingList = m_agendaService.meetingQuery(m_userName, startTime, endTime);
    if (meetingList.empty()) {
        cout << "None" << endl << endl;
    }
    else {
        printMeetings(meetingList);
    }
    m_log.writeLog(m_userName, "query meeting by time", "corresponding meeting", "succeed");
}

void AgendaUI::deleteMeetingByTitle(){
    cout << endl << "[delete meeting] [title]" << endl;
    cout << "[delete meeting] ";
    string title;
    cin >> title;
    if (m_agendaService.deleteMeeting(m_userName, title)) {
        cout << "[delete meeting] succeed!";
        m_log.writeLog(m_userName, "delete meeting by title", "deleted " + title, "succeed");
    }
    else {
        cout << "[error] delete meeting fail!";
        m_log.writeLog(m_userName, "delete meeting by title", title, "fail");
    }
    cout << endl << endl;
}

void AgendaUI::deleteAllMeetings(){
    cout << endl;
    if (m_agendaService.deleteAllMeetings(m_userName)) {
        cout << "[delete all meeting] succeed!";
        m_log.writeLog(m_userName, "delete all meetings", "deleted meetings", "succeed");
    }
    else {
        cout << "[error] delete meeting fail!";
        m_log.writeLog(m_userName, "delete all meetings", "all meetings", "fail");
    }
    cout << endl << endl;
}

string participatorString(vector<string> participators) {
    string res = "";
    for (auto i = participators.begin(); i != participators.end(); ++i) {
        res += *i;
        res += ',';
    }
    res.pop_back();
    return res;
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings){
    cout << setiosflags(ios::left) << setw(15) << "title";
    cout << setw(15) << "sponsor";
    cout << setw(18) << "start time";
    cout << setw(18) << "end time";
    cout << "participators" << endl;
    for (auto i = t_meetings.begin(); i != t_meetings.end(); ++i) {
        cout << setw(15) << i->getTitle();
        cout << setw(15) << i->getSponsor();
        cout << setw(18) << Date::dateToString(i->getStartDate());
        cout << setw(18) << Date::dateToString(i->getEndDate());
        cout << participatorString(i->getParticipator()) << endl;
    }
    cout << endl;
}
