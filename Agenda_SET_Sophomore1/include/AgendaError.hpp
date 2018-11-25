//
//  AgendaError.hpp
//  Agenda
//
//  Created by 许滨楠 on 2018/11/16.
//  Copyright © 2018 Pie. All rights reserved.
//

#ifndef AgendaError_h
#define AgendaError_h

#include <stdexcept>
#include <string>

class AgendaError : public std::exception {
private:
    std::string errorInfo;
public:
    AgendaError();
    AgendaError(int type);
    std::string what();
};

#endif /* AgendaError_h */
