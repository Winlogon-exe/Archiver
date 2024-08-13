//
// Created by winlogon on 06.08.2024.
//

#ifndef DETROIT_MAINFORMLOGIC_H
#define DETROIT_MAINFORMLOGIC_H

#include <QObject>
#include <QDebug>
#include <functional>
#include <QFile>
#include <QByteArray>
#include <QFileInfo>
#include <QDir>

#include <zconf.h>
#include <zlib.h>
#include <zip.h>

enum ButtonsState {
    ARCHIVE_File,
    EXTRACT_File,
    Path_LineEdit
};

class MainFormLogic: public QObject {
    Q_OBJECT

public:
    MainFormLogic();
    ~MainFormLogic() override = default;

private:
    void InitializeMapButton();
    static bool CreateZipFolder(const QString &folderPath, const QString &outputFolderPath);
    static bool CreateZipFile(const QString &filePath, const QString &zipPath);

public slots:
    void ButtonState(QObject* sender, ButtonsState state);
    void ProcessState(QObject* sender);
    static void ArchiveFile(const QString& path);

signals:
    void UpdateFileSystem();
    void ArchiveFileButton();
    void OpenArchiveWindow();

private:
    std::map<ButtonsState, std::function<void()>> logicMap;
    std::map<QObject*, ButtonsState> buttonStateMap;
};

#endif //DETROIT_MAINFORMLOGIC_H
