#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

int main() {
    fs::path sourceDir = "./TestFiles";
    
    // 只处理 .jpg 和 .txt 文件
    for (const auto& entry : fs::directory_iterator(sourceDir)) {
        if (!entry.is_regular_file()) continue; // 跳过文件夹

        std::string ext = entry.path().extension().string();
        std::string targetDir = "Uncategorized";

        if (ext == ".jpg") targetDir = "Images";
        else if (ext == ".txt") targetDir = "Documents";

        // 创建文件夹并移动文件
        fs::create_directories(sourceDir / targetDir);
        fs::rename(entry.path(), sourceDir / targetDir / entry.path().filename());
    }

    std::cout << "整理完成！快去TestFiles文件夹看看吧！" << std::endl;
    return 0;
}