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

    void MainFormLogic::Archive(const QString &absolutePath)
    {
        QThread::sleep(2);
        int errorDir = 0;
        QString zipPath = absolutePath + ".zip";
        zip_t *archive = zip_open(zipPath.toUtf8().constData(), ZIP_CREATE | ZIP_TRUNCATE, &errorDir);

        if (archive == nullptr)
        {
            qDebug() << "Failed to create zip archive:" << zipPath;
            return;
        }

        QFileInfo fileInfo(absolutePath);

        if (fileInfo.isDir())
        {
            AddFolderToZip(archive, absolutePath, fileInfo.fileName());
        }
        else if (fileInfo.isFile())
        {
            AddFileToZip(archive, absolutePath, fileInfo.fileName());
        }

        if (zip_close(archive) < 0)
        {
            qDebug() << "Failed to close zip archive:" << zip_strerror(archive);
        }
    }

    bool MainFormLogic::AddFileToZip(zip_t *archive, const QString &absolutePathFile, const QString &relativePathInArchive)
    {
        QFile file(absolutePathFile);
        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "Failed to open file:" << absolutePathFile;
            return false;
        }

        zip_source_t *source = zip_source_file(archive, absolutePathFile.toUtf8().constData(), 0, 0);

        if (source == nullptr)
        {
            qDebug() << "Failed to create zip source for file:" << absolutePathFile << "Error:" << zip_strerror(archive);
            return false;
        }

        if (zip_file_add(archive, relativePathInArchive.toUtf8().constData(), source, ZIP_FL_ENC_UTF_8) < 0)
        {
            qDebug() << "Failed to add file to zip:" << absolutePathFile << "Error:" << zip_strerror(archive);
            zip_source_free(source);
            return false;
        }

        return true;
    }

    bool MainFormLogic::AddFolderToZip(zip_t *archive, const QString &absolutePathFolder, const QString &relativePathInArchive)
    {
        // Добавляем саму папку
        if (zip_dir_add(archive, relativePathInArchive.toUtf8().constData(), ZIP_FL_ENC_UTF_8) < 0)
        {
            qDebug() << "Failed to add directory to zip:" << relativePathInArchive << "Error:" << zip_strerror(archive);
            return false;
        }

        QDir dir(absolutePathFolder);
        QStringList files = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

        for (const QString &file : files)
        {
            QString absolutePath = absolutePathFolder + "/" + file;
            QString relativePath = relativePathInArchive + "/" + file;
            QFileInfo fileInfo(absolutePath);

            if (fileInfo.isDir())
            {
                // Рекурсивно добавляем содержимое папки
                if (!AddFolderToZip(archive, absolutePath, relativePath))
                {
                    return false;
                }
            }
            else if (fileInfo.isFile())
            {
                if (!AddFileToZip(archive, absolutePath, relativePath))
                {
                    return false;
                }
            }
        }
        return true;
    }
}
