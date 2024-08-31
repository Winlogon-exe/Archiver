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
#include <QFileSystemModel>
#include <QFileInfo>
#include <QDir>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <zconf.h>
#include <zlib.h>
#include <zip.h>

#include "UI/ArchiveExplorer.h"

namespace MainLogic {

    enum ButtonsState {
        ARCHIVE_File,
        UNARCHIVE_File,
        Path_LineEdit,
        Back
    };

    class MainFormLogic : public QObject {
    Q_OBJECT

    public:
        MainFormLogic();
        ~MainFormLogic() override = default;

    private:
        void InitializeMapButton();
        bool AddFileToZip(zip_t *archive,const QString &filePath, const QString &relativePathInArchive);
        bool AddFolderToZip(zip_t *archive,const QString &filePath, const QString &relativePathInArchive);

    public slots:
        void ButtonState(QObject *sender, ButtonsState state);
        void ProcessState(QObject *sender);
        void Archive(const QString &path);
        void UnArchive(const QString &path);
        void FileDoubleClicked(const QModelIndex &index, QFileSystemModel *model);

    signals:
        void UpdateFileSystem();
        void BackFileSystem();
        void ArchiveFileButton();
        void UnarchiveFileButton();
        void OpenArchiveInExplorer(const QString &path);
        void UpdateListView(const QModelIndex &index);

    private:
        std::map<ButtonsState, std::function<void()>> logicMap;
        std::map<QObject *, ButtonsState> buttonStateMap;
        //std::shared_ptr<ArchiveExplorer> archiveExplorer;
    };
}
#endif //DETROIT_MAINFORMLOGIC_H
