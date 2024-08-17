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
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <zconf.h>
#include <zlib.h>
#include <zip.h>

namespace MainLogic {

    enum ButtonsState {
        ARCHIVE_File,
        UNARCHIVE_File,
        Path_LineEdit
    };

    class MainFormLogic : public QObject {
    Q_OBJECT

    public:
        MainFormLogic();
        ~MainFormLogic() override = default;

    private:
        void InitializeMapButton();
        static bool AddFileToZip(zip_t *archive,const QString &filePath, const QString &relativePathInArchive);
        static bool AddFolderToZip(zip_t *archive,const QString &filePath, const QString &relativePathInArchive);

    public slots:
        void ButtonState(QObject *sender, ButtonsState state);
        void ProcessState(QObject *sender);
        static void Archive(const QString &path);
        static void UnArchive(const QString &path);

    signals:
        void UpdateFileSystem();
        void ArchiveFileButton();
        void UnarchiveFileButton();

    private:
        std::map<ButtonsState, std::function<void()>> logicMap;
        std::map<QObject *, ButtonsState> buttonStateMap;
    };
}
#endif //DETROIT_MAINFORMLOGIC_H
