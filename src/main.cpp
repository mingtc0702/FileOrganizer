#include <fstream>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <filesystem>
#include <map>
#include <algorithm>
#include <cctype>
#include <vector>
#include <thread>
#include "FolderWatcher.h"  // 监控功能头文件
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    // ===== 1. 命令行参数处理 =====
    fs::path sourceDir = (argc > 1) ? argv[1] : "./TestFiles";
    std::cout << "整理目录: " << sourceDir << std::endl;
    
    // ===== 2. YAML配置解析 =====
    std::map<std::string, std::string> rules;
    std::vector<std::string> ignoreList = {".DS_Store"};
    std::string defaultDir = "Uncategorized";
    
    try {
        YAML::Node config = YAML::LoadFile("config.yaml");
        
        // 解析分类规则
        if (config["rules"]) {
            for (YAML::const_iterator it = config["rules"].begin(); it != config["rules"].end(); ++it) {
                rules[it->first.as<std::string>()] = it->second.as<std::string>();
            }
        }
        
        // 解析忽略列表
        if (config["ignore"]) {
            ignoreList = config["ignore"].as<std::vector<std::string>>();
        }
        
        // 解析默认目录
        if (config["default_dir"]) {
            defaultDir = config["default_dir"].as<std::string>();
        }
        
        std::cout << "配置文件加载成功！使用自定义规则" << std::endl;
    } catch (...) {
        // 配置文件加载失败时使用默认规则
        rules = {
            {".jpg", "Images"}, {".jpeg", "Images"}, {".png", "Images"},
            {".txt", "Documents"}, {".pdf", "Documents"}, {".docx", "Documents"},
            {".mp3", "Music"}, {".wav", "Music"},
            {".mp4", "Videos"}, {".mov", "Videos"},
            {".zip", "Archives"}, {".rar", "Archives"}
        };
        std::cerr << "警告：配置文件加载失败，使用默认规则" << std::endl;
    }
    
    // ===== 3. 监控模式检查 =====
    if (argc > 2 && std::string(argv[2]) == "--watch") {
        std::cout << "🔍 进入监控模式 (Ctrl+C退出)\n";
        FolderWatcher watcher(sourceDir, rules, ignoreList, defaultDir); // 传入配置
        watcher.start();
        return 0;
    }
    
    // ===== 4. 单次文件整理 =====
    for (const auto& entry : fs::directory_iterator(sourceDir)) {
        if (!entry.is_regular_file()) continue; // 跳过文件夹
        
        std::string filename = entry.path().filename().string();
        std::string ext = entry.path().extension().string();
        
        // 检查是否在忽略列表
        if (std::find(ignoreList.begin(), ignoreList.end(), filename) != ignoreList.end()) {
            continue; // 跳过忽略文件
        }
        
        std::string targetDir = defaultDir; // 使用配置的默认目录

        // 转换扩展名为小写
        std::string extLower = ext;
        std::transform(extLower.begin(), extLower.end(), extLower.begin(), 
                    [](unsigned char c){ return std::tolower(c); });

        // 匹配规则
        auto it = rules.find(extLower);
        if (it != rules.end()) {
            targetDir = it->second;
        }

        // 创建文件夹并移动文件
        fs::create_directories(sourceDir / targetDir);
        fs::rename(entry.path(), sourceDir / targetDir / entry.path().filename());
    }

    std::cout << "整理完成！快去文件夹看看吧！" << std::endl;
    return 0;
}