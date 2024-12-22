//
// Created by winlogon on 06.08.2024.
//

#include "logic/MainForm.h"

namespace MainLogic
{
    MainForm::MainForm()
    {
        InitializeMapButton();
    }

    void MainForm::ButtonState(QObject *sender, ButtonsState state)
    {
        buttonStateMap[sender] = state;
    }

    void MainForm::InitializeMapButton()
    {
        logicMap[ARCHIVE]   = [this]() { emit ArchiveFileButton();};
        logicMap[UNARCHIVE] = [this]() { emit UnArchiveFileButton();};
        logicMap[PATH]      = [this]() { emit UpdateFileSystem();};
        logicMap[BACK]      = [this]() { emit BackFileSystem();};
    }

    void MainForm::ProcessState(QObject *sender)
    {
        ButtonsState state = buttonStateMap[sender];
        auto it = logicMap.find(state);

        if (it != logicMap.end())
        {
            it->second();
        }
    }

    void MainForm::Archive(const QString &absolutePath)
    {
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

    //TODO:: добавить еще параметр куда сохранять сжатый файл
    bool MainForm::AddFileToZip(zip_t *archive, const QString &absolutePathFile, const QString &relativePathInArchive)
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
            qDebug() << "Failed to create zip source for file:" << absolutePathFile << "Error:"<< zip_strerror(archive);
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

    //TODO:: добавить еще параметр куда сохранять сжатый файл
    bool MainForm::AddFolderToZip(zip_t *archive, const QString &absolutePathFolder, const QString &relativePathInArchive)
    {
        if (zip_dir_add(archive, relativePathInArchive.toUtf8().constData(), ZIP_FL_ENC_UTF_8) < 0)
        {
            qDebug() << "Failed to add directory to zip:" << relativePathInArchive << "Error:" << zip_strerror(archive);
            return false;
        }

        QDir dir(absolutePathFolder);
        QStringList files = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

        for (const QString &file: files)
        {
            QString absolutePath = absolutePathFolder + "/" + file;
            QString relativePath = relativePathInArchive + "/" + file;
            QFileInfo fileInfo(absolutePath);

            if (fileInfo.isDir())
            {
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

    void MainForm::UnArchive(const QString &zipPath)
    {
        int errorDir = 0;
        zip_t *archive = zip_open(zipPath.toUtf8().constData(), ZIP_RDONLY, &errorDir);
        if (archive == nullptr)
        {
            qDebug() << "Failed to open zip archive:" << zipPath;
            return;
        }

        zip_int64_t numEntries = zip_get_num_entries(archive, 0);

        QFileInfo fileInfo(zipPath);
        QString destDirPath = fileInfo.absolutePath();

        for (zip_uint64_t i = 0; i < numEntries; i++)
        {
            zip_stat_t st;
            zip_stat_index(archive, i, ZIP_RDONLY, &st);

            QString fileName = QString::fromUtf8(st.name);
            QString filePath = destDirPath + "/" + fileName;

            if (fileName.endsWith("/"))
            {
                QDir().mkpath(filePath);
            }
            else
            {
                zip_file_t *zf = zip_fopen_index(archive, i, 0);
                QFile file(filePath);
                file.open(QIODevice::WriteOnly);

                char buffer[8192];
                zip_int64_t bytesRead = 0;

                while ((bytesRead = zip_fread(zf, buffer, sizeof(buffer))) > 0)
                {
                    file.write(buffer, bytesRead);
                }
                file.close();
                zip_fclose(zf);
            }
        }
        zip_close(archive);
        qDebug() << "Unzipping completed to:" << destDirPath;
    }

    void MainForm::FileDoubleClicked(const QModelIndex &index, QFileSystemModel *fileSystemModel)
    {
        QString selectedFilePath = fileSystemModel->filePath(index);

        if (selectedFilePath.endsWith(".zip") || selectedFilePath.endsWith(".rar"))
        {
            emit OpenArchiveInExplorer(selectedFilePath);
        }
        else if (QFileInfo(selectedFilePath).isFile())
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(selectedFilePath));
        }
        else if (QFileInfo(selectedFilePath).isDir())
        {
            emit UpdateListView(index);
        }
    }
}
