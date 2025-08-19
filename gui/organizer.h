#ifndef ORGANIZER_H
#define ORGANIZER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <filesystem>

namespace fs = std::filesystem;

class Organizer : public QObject
{
    Q_OBJECT
public:
    explicit Organizer(QObject *parent = nullptr);
    ~Organizer();

    // 核心功能接口
    void organizeDirectory(const QString &path);
    void startWatching(const QString &path);
    void stopWatching();

signals:
    void progressChanged(int percent);
    void finished();

private:
    QFileSystemWatcher *watcher;
    fs::path currentPath;
};

#endif // ORGANIZER_H
