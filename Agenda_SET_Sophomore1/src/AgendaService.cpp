//
//  AgendaService.cpp
//  Agenda
//
//  Created by 许滨楠 on 2018/11/10.
//  Copyright © 2018 Pie. All rights reserved.
//

#include "AgendaService.hpp"
#include "Date.hpp"
#include "Meeting.hpp"
#include "Storage.hpp"
#include "User.hpp"

#include <string>
#include <list>

/**
 * constructor
 */
AgendaService::AgendaService() {
    startAgenda();
}

/*
 * destructor
 */
AgendaService::~AgendaService() {
    quitAgenda();
}

/*
 * check if the username match password
 * @param userName the username want to login
 * @param password the password user enter
 * @return if success, true will be returned
 */
bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
    std::list<User> ul;
    ul = m_storage->queryUser([userName, password](const User &qu) {
        return (qu.getName() == userName && qu.getPassword() == password);
    });
    return (!ul.empty());
}

/*
 * regist a user
 * @param userName new user's username
 * @param password new user's password
 * @param email new user's email
 * @param phone new user's phone
 * @return if success, true will be returned
 */
bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                                 const std::string &email, const std::string &phone) {
    std::list<User> ul = m_storage->queryUser([userName](const User &qu) {
        return (qu.getName() == userName);
    });
    if (!ul.empty()) return false;
    User t(userName, password, email, phone);
    m_storage->createUser(t);
    return true;
}

/*
 * delete a user
 * @param userName user's username
 * @param password user's password
 * @return if success, true will be returned
 */
bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
    auto userFilter = [userName,password](const User & qu) {
        return (qu.getName() == userName && qu.getPassword() == password);
    };
    auto meetingFilter = [userName](const Meeting & qm) {
        return (qm.isParticipator(userName));
    };
    auto removePar = [userName](Meeting & qm) {
        qm.removeParticipator(userName);
    };
    auto noPar = [](const Meeting & qm) {
        return (qm.getParticipator().size() == 0);
    };
    
    int count = m_storage->deleteUser(userFilter);
    if (count == 0) {
        return false;
    }
    else {
        deleteAllMeetings(userName);
        m_storage->updateMeeting(meetingFilter, removePar);
        m_storage->deleteMeeting(noPar);
        return true;
    }
}

/*
 * list all users from storage
 * @return a user list result
 */
std::list<User> AgendaService::listAllUsers(void) const {
    return m_storage->queryUser([](const User &qu) {
        return true;
    });
}

/*
 * create a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @param startDate the meeting's start date
 * @param endDate the meeting's end date
 * @return if success, true will be returned
 */
bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                                  const std::string &startDate, const std::string &endDate,
                                  const std::vector<std::string> &participator) {
    for (auto i = participator.begin(); i != participator.end(); ++i) {
        for (auto j = i + 1; j != participator.end(); ++j) {
            if (*i == *j) return false;
        }
    }
    Date start;
    Date end;
    start = Date::stringToDate(startDate);
    end = Date::stringToDate(endDate);
    if (!(Date::isValid(start) && Date::isValid(end) && start < end)) return false;
    std::list<User> ul;
    std::list<Meeting> ml;
    //if the user has registered
    ul = m_storage->queryUser([userName](const User &qu) {
        return (qu.getName() == userName);
    });
    if (ul.empty()) return false;
    
    //check the time plan of the sponsor
    ml = m_storage->queryMeeting([userName, start, end](const Meeting &qm) {
        return ((qm.getSponsor() == userName || qm.isParticipator(userName))&&
                //the user has more than one meeting, go into next judge to see the time scale
                ((qm.getStartDate() <= start && qm.getEndDate() > start)||
                 (qm.getStartDate() < end && qm.getEndDate() >= end)||
                 (qm.getStartDate() >= start && qm.getEndDate() <= end)));
                //better time judge in the next function addMeetingParticipator
    });
    if (!ml.empty()) return false;
    
    //check the time plan of the participators
    for (auto i = participator.begin(); i != participator.end(); ++i) {
        if (*i == userName) return false;
        std::string tuserName = *i;
        ul = m_storage->queryUser([tuserName](const User &qu) {
            return (qu.getName() == tuserName);
        });
        if (ul.empty()) return false;
        ml = m_storage->queryMeeting([tuserName, start, end](const Meeting &qm) {
            return ((qm.getSponsor() == tuserName || qm.isParticipator(tuserName))&&
                    //the participator has more than one meeting, go into next judge to see the time scale
                    ((qm.getStartDate() <= start && qm.getEndDate() > start)||
                     (qm.getStartDate() < end && qm.getEndDate() >= end)||
                     (qm.getStartDate() >= start && qm.getEndDate() <= end)));
                    //better time judge in the next function addMeetingParticipator
        });
        if (!ml.empty()) return false;
    }
    
    //check if there are repeated titles
    ml = m_storage->queryMeeting([title](const Meeting &qm) {
        return (qm.getTitle() == title);
    });
    if (!ml.empty()) return false;
    Meeting t(userName, participator, start, end, title);
    m_storage->createMeeting(t);
    return true;
}

/*
 * add a participator to a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @return if success, true will be returned
 */
bool AgendaService::addMeetingParticipator(const std::string &userName,
                                           const std::string &title,
                                           const std::string &participator) {
    auto userFilter = [participator](const User & qu) {
        return (qu.getName() == participator);
    };
    if (m_storage->queryUser(userFilter).size() == 0) return false;
    std::list<Meeting> checkTime = listAllMeetings(participator);
    auto filter = [userName, title, participator, checkTime](const Meeting &um) {
        Date start = um.getStartDate();
        Date end = um.getEndDate();
        if (um.getSponsor() == userName && um.getTitle() == title && (!um.isParticipator(participator))) {
            for (auto i = checkTime.begin(); i != checkTime.end(); ++i) {
                if (!(end <= i->getStartDate() || start >= i->getEndDate())) return false;
            }
            return true;
        }
        return false;
    };
    auto switcher = [participator](Meeting &um) {
        um.addParticipator(participator);
    };
    int count = m_storage->updateMeeting(filter, switcher);
    return (count > 0);
}

/*
 * remove a participator from a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @return if success, true will be returned
 */
bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                              const std::string &title,
                                              const std::string &participator) {
    auto filter = [userName, title, participator](const Meeting &um){
        return (um.getSponsor() == userName && um.getTitle() == title && um.isParticipator(participator));
    };
    auto switcher = [participator](Meeting &um){
        um.removeParticipator(participator);
    };
    int flag = m_storage->updateMeeting(filter, switcher);
    if (flag > 0) {
        m_storage->deleteMeeting([](const Meeting &dm) {
            return ((dm.getParticipator()).size() == 0);
        });
        return true;
    }
    return false;
}

/*
 * quit from a meeting
 * @param userName the current userName. need to be the participator (a sponsor can not quit his/her meeting)
 * @param title the meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    auto filter = [userName, title](const Meeting &um){
        if (um.getSponsor() == userName) return false;
        return (um.isParticipator(userName) && um.getTitle() == title);
    };
    auto switcher = [userName](Meeting &um){
        um.removeParticipator(userName);
    };
    int flag = m_storage->updateMeeting(filter, switcher);
    if (flag > 0) {
        m_storage->deleteMeeting([](const Meeting &dm) {
            return ((dm.getParticipator()).size() == 0);
        });
        return true;
    }
    return false;
}

/*
 * search a meeting by username and title
 * @param userName as a sponsor OR a participator
 * @param title the meeting's title
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &title) const {
    return m_storage->queryMeeting([userName, title](const Meeting &qm) {
        return ((qm.getSponsor() == userName || qm.isParticipator(userName))&&(qm.getTitle() == title));
    });
}
/*
 * search a meeting by username, time interval
 * @param userName as a sponsor OR a participator
 * @param startDate time interval's start date
 * @param endDate time interval's end date
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &startDate,
                                               const std::string &endDate) const {
    Date start(startDate);
    Date end(endDate);
    if ((!(Date::isValid(start) && (Date::isValid(end))))||(start > end)) {
        std::list<Meeting> ml;
        return ml;
    }
    return m_storage->queryMeeting([userName, start, end](const Meeting &qm) {
        return ((qm.getSponsor() == userName || qm.isParticipator(userName))&&
                (!(end < qm.getStartDate() || start > qm.getEndDate())));
    });
}

/*
 * list all meetings the user take part in
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
    return m_storage->queryMeeting([&userName](const Meeting &qm) {
        return (qm.getSponsor() == userName || qm.isParticipator(userName));
    });
}

/*
 * list all meetings the user sponsor
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
    return m_storage->queryMeeting([&userName](const Meeting &qm) {
        return (qm.getSponsor() == userName);
    });
}

/*
 * list all meetings the user take part in and sponsor by other
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const {
    return m_storage->queryMeeting([&userName](const Meeting &qm) {
        return (qm.isParticipator(userName));
    });
}

/*
 * delete a meeting by title and its sponsor
 * @param userName sponsor's username
 * @param title meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
    int flag = m_storage->deleteMeeting([&userName, &title](const Meeting &dm) {
        return (dm.getSponsor() == userName && dm.getTitle() == title);
    });
    return (flag > 0);
}

/*
 * delete all meetings by sponsor
 * @param userName sponsor's username
 * @return if success, true will be returned
 */
bool AgendaService::deleteAllMeetings(const std::string &userName) {
    int flag = m_storage->deleteMeeting([&userName](const Meeting &dm) {
        return (dm.getSponsor() == userName);
    });
    return (flag > 0);
}

/*
 * start Agenda service and connect to storage
 */
void AgendaService::startAgenda(void) {
    m_storage = Storage::getInstance();
}

/*
 * quit Agenda service
 */
void AgendaService::quitAgenda(void) {
    m_storage->sync();
}
