#pragma once
#include <fstream>
#include <string>
#include <ctime>

class Logger {
public:
    static void log(const std::string& message);
};
