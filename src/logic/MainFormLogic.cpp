//
// Created by winlogon on 06.08.2024.
//

#include "logic/MainFormLogic.h"

MainFormLogic::MainFormLogic() {
    InitializeMapButton();
}

void MainFormLogic::ButtonState(QObject* sender, ButtonsState state) {
    buttonStateMap[sender] = state;
}

//сигнал из логики должен идти только если он обновляет UI, то есть если логика не требует полей из ui то выполняется здесь
void MainFormLogic::InitializeMapButton() {
    logicMap[ARCHIVE_File]  = [this]()  { emit ArchiveFileButton(); };
    logicMap[EXTRACT_File]  = [this]()  { };
    logicMap[Path_LineEdit] = [this]()  { emit UpdateFileSystem();};
}

void MainFormLogic::ProcessState(QObject *sender) {
    ButtonsState state = buttonStateMap[sender];
    auto it = logicMap.find(state);

    if (it != logicMap.end())
    {
        it->second();
    }
}

void MainFormLogic::ArchiveFile(const QString &path) {
    QFileInfo info(path);

    if (info.isFile()) {
        QString zipPath = path + ".zip";
        if (CreateZipArchive(path, zipPath)) {
            qDebug() << "File successfully archived to" << zipPath;
        } else {
            qDebug() << "Failed to archive file";
        }
    } else if (info.isDir()) {
        qDebug() << "Directory archiving is not implemented in this example";
    }
}

void MainFormLogic::CompressFile(const QString &filePath, const QString &outputFilePath) {

}

void MainFormLogic::CompressFolder(const QString &folderPath, const QString &outputFolderPath) {

}

bool MainFormLogic::CompressData(const QByteArray &data, QByteArray &compressedData) {

}

bool MainFormLogic::CreateZipArchive(const QString &filePath, const QString &zipPath) {
    int error = 0;
    zip_t *archive = zip_open(zipPath.toUtf8().constData(), ZIP_CREATE | ZIP_TRUNCATE, &error);

    if (archive == nullptr) {
        qDebug() << "Failed to create ZIP archive with error code" << error;
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading:" << file.errorString();
        zip_discard(archive);
        return false;
    }

    QByteArray fileData = file.readAll();
    zip_source_t *source = zip_source_buffer(archive, fileData.data(), fileData.size(), 0);

    if (source == nullptr) {
        qDebug() << "Failed to create ZIP source";
        zip_discard(archive);
        return false;
    }

    if (zip_file_add(archive, QFileInfo(filePath).fileName().toUtf8().constData(), source, ZIP_FL_OVERWRITE) < 0) {
        qDebug() << "Failed to add file to ZIP archive";
        zip_source_free(source);
        zip_discard(archive);
        return false;
    }

    if (zip_close(archive) < 0) {
        qDebug() << "Failed to close ZIP archive";
        return false;
    }

    return true;
}

