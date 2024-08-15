//
// Created by winlogon on 06.08.2024.
//

#include "logic/MainFormLogic.h"

namespace MainLogic
{
    MainFormLogic::MainFormLogic()
    {
        InitializeMapButton();
    }

    void MainFormLogic::ButtonState(QObject *sender, ButtonsState state)
    {
        buttonStateMap[sender] = state;
    }

    void MainFormLogic::InitializeMapButton()
    {
        logicMap[ARCHIVE_File] = [this]()  { emit ArchiveFileButton(); };
        logicMap[EXTRACT_File] = [this]()  { emit OpenArchiveWindow(); };
        logicMap[Path_LineEdit] = [this]() { emit UpdateFileSystem(); };
    }

    void MainFormLogic::ProcessState(QObject *sender)
    {
        ButtonsState state = buttonStateMap[sender];
        auto it = logicMap.find(state);

        if (it != logicMap.end())
        {
            it->second();
        }
    }

    void MainFormLogic::Archive(const QString &path)
    {
        QFileInfo info(path);
        QString zipPath = path + ".zip";

        if (info.isFile() && AddFileToZip(path, zipPath))
        {
            qDebug() << "File successfully archived to" << zipPath;
            return;
        }

        if (info.isDir() && CreateZipFolder(path,zipPath))
        {
            qDebug() << "Folder successfully archived to" << zipPath;
            return;
        }
        qDebug() << "Failed to archive file or folder";
    }

    bool MainFormLogic::CreateZipFolder(const QString &absolutePathFolder, const QString &zipPath)
    {
        int errorDir = 0;
        zip_t *archive = zip_open(zipPath.toUtf8().constData(), ZIP_CREATE | ZIP_TRUNCATE, &errorDir);

        QString baseFolderName = QFileInfo(absolutePathFolder).fileName();
        if (zip_dir_add(archive, baseFolderName.toUtf8().constData(), ZIP_FL_ENC_UTF_8) < 0)
        {
            qDebug() << "Failed to add directory to zip:" << baseFolderName << "Error:" << zip_strerror(archive);
            zip_discard(archive);
            return false;
        }

        QDir dir(absolutePathFolder);
        QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QFileInfo &fileInfo : fileList)
        {
            if (fileInfo.isDir())
            {
                qDebug()<<"not implemented";
            }
            else
            {
                AddFileToZip(fileInfo.absoluteFilePath(), zipPath);
            }
        }
        return true;
    }

    bool MainFormLogic::AddFileToZip(const QString &absolutePathFile, const QString &zipPath)
    {
        int error = 0;
        zip_t *archive = zip_open(zipPath.toUtf8().constData(), ZIP_CREATE | ZIP_TRUNCATE, &error);

        QFile file(absolutePathFile);
        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "Failed to open file for reading:" << file.errorString();
            zip_discard(archive);
            return false;
        }

        QByteArray fileData = file.readAll();
        zip_source_t *source = zip_source_buffer(archive, fileData.data(), fileData.size(), 0);

        if (zip_file_add(archive, QFileInfo(absolutePathFile).fileName().toUtf8().constData(), source, ZIP_FL_OVERWRITE) < 0)
        {
            qDebug() << "Failed to add file to ZIP archive";
            zip_source_free(source);
            zip_discard(archive);
            return false;
        }

        if (zip_close(archive) < 0)
        {
            qDebug() << "Failed to close ZIP archive";
            return false;
        }
        return true;
    }
}
