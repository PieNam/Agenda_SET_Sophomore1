//
//  Path.hpp
//  Agenda
//
//  Created by 许滨楠 on 2018/11/10.
//  Copyright © 2018 Pie. All rights reserved.
//

#ifndef PATH_HPP
#define PATH_HPP

class Path {
public:
    /*
     * user.csv path
     */
    static const char *userPath;
    
    /*
     * meeting.csv path
     */
    static const char *meetingPath;
};

const char *Path::meetingPath = "./tmp/meetings.csv";
const char *Path::userPath = "./tmp/users.csv";

#endif
