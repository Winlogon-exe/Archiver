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
    void CompressData(const QByteArray &data, QByteArray &compressedData);
    void CompressFile(const QString &filePath, const QString &outputFilePath);
    void CompressFolder(const QString &folderPath, const QString &outputFolderPath);
    void CreateZipArchive(const QString &zipFilePath, const QString &fileName,const std::vector<unsigned char>&data);

public slots:
    void ButtonState(QObject* sender, ButtonsState state);
    void ProcessState(QObject* sender);
    void ArchiveFile(const QString& path);

signals:
    void UpdateFileSystem();
    void ArchiveFileButton();

private:
    std::map<ButtonsState, std::function<void()>> logicMap;
    std::map<QObject*, ButtonsState> buttonStateMap;
};

#endif //DETROIT_MAINFORMLOGIC_H
