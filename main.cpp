#include <iostream>
#include <filesystem>
#include <map>         
#include <algorithm>    
#include <cctype>       
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    // 支持命令行参数
    fs::path sourceDir = (argc > 1) ? argv[1] : "./TestFiles";
    std::cout << "整理目录: " << sourceDir << std::endl;
    
    // 文件类型规则映射
    std::map<std::string, std::string> rules = {
        {".jpg", "Images"}, {".jpeg", "Images"}, {".png", "Images"},
        {".txt", "Documents"}, {".pdf", "Documents"}, {".docx", "Documents"},
        {".mp3", "Music"}, {".wav", "Music"},
        {".mp4", "Videos"}, {".mov", "Videos"},
        {".zip", "Archives"}, {".rar", "Archives"}
    };
    
    // 只处理文件
    for (const auto& entry : fs::directory_iterator(sourceDir)) {
        if (!entry.is_regular_file()) continue; // 跳过文件夹

        std::string ext = entry.path().extension().string();
        std::string targetDir = "Uncategorized";

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