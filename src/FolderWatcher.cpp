#include "FolderWatcher.h"
#include "../include/Logger.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <thread> 

FolderWatcher::FolderWatcher(const fs::path& path, 
                             const std::map<std::string, std::string>& rules,
                             const std::vector<std::string>& ignoreList,
                             const std::string& defaultDir)
    : target_path(path), 
      rules(rules),
      ignoreList(ignoreList),
      defaultDir(defaultDir) {}

void FolderWatcher::start(int interval_sec) {
    Logger::log("启动目录监控: " + target_path.string());
    std::cout << "🕒 开始监控: " << target_path << std::endl;
    
    while (true) {
        for (const auto& entry : fs::directory_iterator(target_path)) {
            if (!entry.is_regular_file()) continue;
            
            const std::string filename = entry.path().filename().string();
            const std::string file_path = entry.path().string();
            
            // 跳过已处理文件
            if (processed_files.find(file_path) != processed_files.end()) continue;
            
            // 检查忽略列表
            if (std::find(ignoreList.begin(), ignoreList.end(), filename) != ignoreList.end()) {
                continue;
            }
            
            std::string ext = entry.path().extension().string();
            std::string targetDir = defaultDir;
            
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
            try {
                fs::create_directories(target_path / targetDir);
                fs::rename(entry.path(), target_path / targetDir / entry.path().filename());
                
                Logger::log("已移动: " + filename + " -> " + targetDir);
                std::cout << "✅ 已整理: " << filename << " -> " << targetDir << std::endl;
            } catch (const fs::filesystem_error& e) {
                Logger::log("移动失败: " + std::string(e.what()));
                std::cerr << "❌ 错误: " << e.what() << std::endl;
            }
            
            processed_files.insert(file_path);
        }
        std::this_thread::sleep_for(std::chrono::seconds(interval_sec));
    }
}