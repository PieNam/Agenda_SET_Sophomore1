//
//  AgendaError.cpp
//  Agenda
//
//  Created by 许滨楠 on 2018/11/16.
//  Copyright © 2018 Pie. All rights reserved.
//

#include "AgendaError.hpp"

#include <stdexcept>
#include <string>

AgendaError::AgendaError() {
    errorInfo = "Error!";
}

AgendaError::AgendaError(int type) {
    switch(type) {
        case 0:
            errorInfo = "User not found!";
            break;
        case 1:
            errorInfo = "Incorrect password!";
            break;
        default:
            errorInfo = "Error!";
    }
}

std::string AgendaError::what() {
    return errorInfo;
}
