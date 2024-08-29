//
// Created by winlogon on 19.08.2024.
//

#ifndef DETROIT_ARCHIVEEXPLORER_H
#define DETROIT_ARCHIVEEXPLORER_H

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTreeView>
#include <QStandardItem>
#include <QDesktopServices>
#include <QTemporaryFile>
#include <QTemporaryDir>
#include <QProcess>
#include <zconf.h>
#include <zlib.h>
#include <zip.h>

class ArchiveExplorer : public QWidget {
    Q_OBJECT

public:
    explicit ArchiveExplorer(QWidget *parent = nullptr);
    ~ArchiveExplorer();

public:
    void SetupConnect();
    void SetupTreeView(const QString &file);
    void SetupLayout();
    QList<QString> getArchiveFileList(const QString &archivePath);

public slots:
    void OpenArchiveExplorer(const QString &file);
    void OpenFileInArchive(const QModelIndex &index);
    void OpenFileInExplorer(const QString& pathToFile);
    void OpenDirInExplorer(const QString& pathToDir);

signals:

private:
    QTreeView *treeView;
    QStandardItemModel *model;
    QStandardItem *item;
};


#endif //DETROIT_ARCHIVEEXPLORER_H
