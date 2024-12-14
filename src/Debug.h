//
// Created by Bradley Remedios on 11/24/24.
//
#ifndef TEMP_CONTROL_DEBUG_H
#define TEMP_CONTROL_DEBUG_H

#include <iostream>

//#define __ENABLE_TEMP_CONTROL_DEBUG    1

#define ERROR_MSG(msg)  { std::cerr << "ERROR: " << __PRETTY_FUNCTION__ << ":" << msg << std::endl; }

#if defined(__ENABLE_TEMP_CONTROL_DEBUG)
    #define DEBUG_MSG(msg)  { std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << ":" << msg << std::endl; }
#else
    #define DEBUG_MSG(cmd)
#endif // __ENABLE_TEMP_CONTROL_DEBUG

#endif //TEMP_CONTROL_DEBUG_H