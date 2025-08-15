#include "Logger.h"
#include <iostream>

void Logger::log(const std::string& message) {
    std::ofstream logfile("organizer.log", std::ios::app);
    if (!logfile) {
        std::cerr << "无法打开日志文件！" << std::endl;
        return;
    }
    
    time_t now = time(0);
    char* dt = ctime(&now);
    logfile << "[" << dt << "] " << message << "\n";
    logfile.close();
}
