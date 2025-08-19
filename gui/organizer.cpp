#include "organizer.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

// 解析配置文件路径：优先 .app/Contents/Resources/config.yaml
static QString resolveConfigPath() {
#ifdef Q_OS_MAC
    {
        // 当从 .app 双击启动时，config.yaml 会被打进 Resources
        const QString inBundle = QCoreApplication::applicationDirPath()
                                 + "/../Resources/config.yaml";
        if (QFileInfo::exists(inBundle)) return QDir(inBundle).absolutePath();
    }
#endif
    {
        const QString besideExe = QCoreApplication::applicationDirPath()
                                  + "/config.yaml";
        if (QFileInfo::exists(besideExe)) return QDir(besideExe).absolutePath();
    }
    {
        // 当前工作目录回退
        const QString inCwd = QDir::current().filePath("config.yaml");
        if (QFileInfo::exists(inCwd)) return QDir(inCwd).absolutePath();
    }
    return {};
}

Organizer::Organizer(QObject *parent)
    : QObject(parent), watcher(new QFileSystemWatcher(this))
{
    // 目录变更即重新整理
    connect(watcher, &QFileSystemWatcher::directoryChanged, this,
            [this](const QString &path) {
                organizeDirectory(path);
            });
}

Organizer::~Organizer()
{
    delete watcher;
}

void Organizer::organizeDirectory(const QString &path)
{
    try {
        // ===== 1) 读取配置 =====
        std::map<std::string, std::string> rules;
        std::vector<std::string> ignoreList = {".DS_Store"};
        std::string defaultDir = "Uncategorized";

        try {
            const QString cfgPath = resolveConfigPath();
            if (!cfgPath.isEmpty()) {
                YAML::Node config = YAML::LoadFile(cfgPath.toStdString());

                if (config["rules"]) {
                    for (auto it = config["rules"].begin();
                         it != config["rules"].end(); ++it) {
                        rules[it->first.as<std::string>()] =
                            it->second.as<std::string>();
                    }
                }
                if (config["ignore"]) {
                    ignoreList = config["ignore"].as<std::vector<std::string>>();
                }
                if (config["default_dir"]) {
                    defaultDir = config["default_dir"].as<std::string>();
                }
            } else {
                // 没找到配置文件，走默认规则
                rules = {
                    {".jpg", "Images"}, {".jpeg", "Images"}, {".png", "Images"},
                    {".txt", "Documents"}, {".pdf", "Documents"}, {".docx", "Documents"},
                    {".mp3", "Music"}, {".wav", "Music"},
                    {".mp4", "Videos"}, {".mov", "Videos"},
                    {".zip", "Archives"}, {".rar", "Archives"}
                };
            }
        } catch (...) {
            // YAML 解析失败：回退默认规则
            rules = {
                {".jpg", "Images"}, {".jpeg", "Images"}, {".png", "Images"},
                {".txt", "Documents"}, {".pdf", "Documents"}, {".docx", "Documents"},
                {".mp3", "Music"}, {".wav", "Music"},
                {".mp4", "Videos"}, {".mov", "Videos"},
                {".zip", "Archives"}, {".rar", "Archives"}
            };
        }

        // ===== 2) 整理逻辑 =====
        fs::path sourceDir = path.toStdString();
        int totalFiles = 0, processedFiles = 0;

        for (const auto &entry : fs::directory_iterator(sourceDir)) {
            if (entry.is_regular_file()) totalFiles++;
        }
        if (totalFiles == 0) {
            emit progressChanged(100);
            emit finished();
            return;
        }

        for (const auto &entry : fs::directory_iterator(sourceDir)) {
            if (!entry.is_regular_file()) continue;

            std::string filename = entry.path().filename().string();
            std::string ext = entry.path().extension().string();

            // 忽略名单
            if (std::find(ignoreList.begin(), ignoreList.end(), filename) != ignoreList.end()) {
                continue;
            }

            std::string targetDir = defaultDir;

            // 扩展名小写化
            std::string extLower = ext;
            std::transform(extLower.begin(), extLower.end(), extLower.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            // 匹配规则
            auto it = rules.find(extLower);
            if (it != rules.end()) targetDir = it->second;

            // 创建目标目录并移动
            fs::create_directories(sourceDir / targetDir);
            fs::rename(entry.path(), sourceDir / targetDir / entry.path().filename());

            // 进度
            processedFiles++;
            int progress = (processedFiles * 100) / totalFiles;
            emit progressChanged(progress);
            QCoreApplication::processEvents(); // 让 UI 更及时
        }

        emit finished();
    } catch (const std::exception &e) {
        qWarning() << "整理出错:" << e.what();
    }
}

void Organizer::startWatching(const QString &path)
{
    // 先清理，再添加，避免重复
    if (!watcher->directories().isEmpty())
        watcher->removePaths(watcher->directories());

    if (!path.isEmpty())
        watcher->addPath(path);

    currentPath = path.toStdString();
}

void Organizer::stopWatching()
{
    if (!watcher->directories().isEmpty())
        watcher->removePaths(watcher->directories());
}
