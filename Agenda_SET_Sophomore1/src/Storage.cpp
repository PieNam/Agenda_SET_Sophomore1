//
//  Storage.cpp
//  Agenda
//
//  Created by 许滨楠 on 2018/11/10.
//  Copyright © 2018 Pie. All rights reserved.
//

#include "Storage.hpp"
#include "Path.hpp"
#include "Date.hpp"
#include "User.hpp"
#include "Meeting.hpp"

#include <functional>
#include <list>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>

std::shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() {
    m_dirty = false;
    readFromFile();
}
/*
 *   read file content into memory
 *   @return if success, true will be returned
 */
bool Storage::readFromFile() {
    std::ifstream userFile(Path::userPath, std::ios::in);
    std::ifstream meetingFile(Path::meetingPath, std::ios::in);
    
    std::string str;
    
    while (std::getline(userFile, str)) {
        std::string keyword[4];
        size_t count = 0;
        for (auto c : str) {
            if (c != '"') {
                if (c == ',') ++count;
                else (keyword[count] += c);
            }
        }
        m_userList.push_back(User(keyword[0], keyword[1], keyword[2], keyword[3]));
    }
    userFile.close();
    
    while (std::getline(meetingFile, str)) {
        std::string keyword[5];
        std::vector<std::string> participators;
        int count = 0;
        for (auto c : str) {
            if (c != '"') {
                if (c == ',') ++count;
                else (keyword[count] += c);
            }
        }
        std::string t;
        for (auto c : keyword[1]) {
            if (c != '&') {
                t += c;
            }
            else {
                participators.push_back(t);
                t = "";
            }
        }
        participators.push_back(t);
        m_meetingList.push_back(Meeting(keyword[0], participators,
                                        Date::stringToDate(keyword[2]),
                                        Date::stringToDate(keyword[3]),
                                        keyword[4]));
    }
    meetingFile.close();
    return true;
}

/*
 *   write file content from memory
 *   @return if success, true will be returned
 */
bool Storage::writeToFile() {
    std::ofstream userFile(Path::userPath, std::ios::out);
    std::ofstream meetingFile(Path::meetingPath, std::ios::out);
    
    for(auto &i : m_userList) {
        userFile << "\"" << i.getName() << "\","
                 << "\"" << i.getPassword() << "\","
                 << "\"" << i.getEmail() << "\","
                 << "\"" << i.getPhone() << "\""
                 << std::endl;
    }
    userFile.close();
    
    for (auto &i : m_meetingList) {
        meetingFile << "\"" << i.getSponsor() << "\","
                    << "\"";
        std::string parString = "";
        for (auto &j : i.getParticipator()) {
            parString.append(j);
            parString.push_back('&');
        }
        parString.pop_back();
        meetingFile << parString << "\",";
        meetingFile << "\"" << Date::dateToString(i.getStartDate()) << "\","
                    << "\"" << Date::dateToString(i.getEndDate()) << "\","
                    << "\"" << i.getTitle() << "\""
                    << std::endl;
    }
    meetingFile.close();
    
    m_dirty = false;
    return true;
}

/*
 * get Instance of storage
 * @return the pointer of the instance
 */
std::shared_ptr<Storage> Storage::getInstance() {
    if (m_instance == nullptr)
        m_instance = std::shared_ptr<Storage>(new Storage);
    return m_instance;
}

/*
 *   destructor
 */
Storage::~Storage() {
    sync();
}

// CRUD for User & Meeting
// using C++11 Function Template and Lambda Expressions

/*
 * create a user
 * @param a user object
 */
void Storage::createUser(const User &t_user) {
    m_userList.push_back(t_user);
    m_dirty = true;
}

/*
 * query users
 * @param a lambda function as the filter
 * @return a list of fitted users
 */
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
    std::list<User> qu;
    for (const User &i : m_userList) {
        if (filter(i)) qu.push_back(i);
    }
    return qu;
}

/*
 * update users
 * @param a lambda function as the filter
 * @param a lambda function as the method to update the user
 * @return the number of updated users
 */
int Storage::updateUser(std::function<bool(const User &)> filter,
                        std::function<void(User &)> switcher) {
    int count = 0;
    for(User &i : m_userList) {
        if(filter(i)) {
            switcher(i);
            ++count;
        }
    }
    if (count != 0) m_dirty = true;
    return count;
}

/*
 * delete users
 * @param a lambda function as the filter
 * @return the number of deleted users
 */
int Storage::deleteUser(std::function<bool(const User &)> filter) {
    int count = 0;
    for (auto i = m_userList.cbegin(); i != m_userList.cend();) {
        if (filter(*i)) {
            i = m_userList.erase(i);
            ++count;
        }
        else {
            ++i;
        }
    }
    if (count != 0) m_dirty = true;
    return count;
}

/*
 * create a meeting
 * @param a meeting object
 */
void Storage::createMeeting(const Meeting & t_meeting) {
    m_meetingList.push_back(t_meeting);
    m_dirty = true;
}
/*
 * query meetings
 * @param a lambda function as the filter
 * @return a list of fitted meetings
 */
std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const {
    std::list<Meeting> qm;
    for (const Meeting &i : m_meetingList) {
        if(filter(i)) qm.push_back(i);
    }
    return qm;
}

/*
 * update meetings
 * @param a lambda function as the filter
 * @param a lambda function as the method to update the meeting
 * @return the number of updated meetings
 */
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                           std::function<void(Meeting &)> switcher) {
    int count = 0;
    for (Meeting &i : m_meetingList) {
        if(filter(i)) {
            switcher(i);
            ++count;
        }
    }
    if (count != 0) m_dirty = true;
    return count;
}

/*
 * delete meetings
 * @param a lambda function as the filter
 * @return the number of deleted meetings
 */
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
    int count = 0;
    for (auto i = m_meetingList.cbegin(); i!= m_meetingList.cend(); ) {
        if(filter(*i)) {
            i = m_meetingList.erase(i);
            ++count;
        }
        else {
            ++i;
        }
    }
    if (count != 0) m_dirty = true;
    return count;
}

/*
 * sync with the file
 */
bool Storage::sync() {
    if (m_dirty) return writeToFile();
    return true;
}
