#pragma once
#include <filesystem>
#include <chrono>
#include <unordered_set>
#include <map>
#include <vector>
#include <string>
namespace fs = std::filesystem;

class FolderWatcher {
public:
    FolderWatcher(const fs::path& path, 
                 const std::map<std::string, std::string>& rules,
                 const std::vector<std::string>& ignoreList,
                 const std::string& defaultDir);
    
    void start(int interval_sec = 5);  // 默认5秒扫描一次
    
private:
    fs::path target_path;
    std::unordered_set<std::string> processed_files;
    std::map<std::string, std::string> rules;
    std::vector<std::string> ignoreList;
    std::string defaultDir;
};