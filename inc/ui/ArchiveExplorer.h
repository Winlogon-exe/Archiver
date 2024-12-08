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
#include <QStandardItemModel>
#include <QTemporaryFile>
#include <QByteArray>
#include <QTemporaryDir>
#include <QProcess>
#include <zconf.h>
#include <zlib.h>
#include <zip.h>
#include <stdio.h>
#include <QMimeDatabase>
#include <QMimeType>
#include <QTextEdit>
#include <QLabel>
#include <QPixmap>
#include <QProcess>
#include <QMessageBox>
#include <QTimer>

class ArchiveExplorer : public QWidget {
    Q_OBJECT

public:
    explicit ArchiveExplorer(QWidget *parent = nullptr);
    ~ArchiveExplorer();

public:
    void InitUI();
    void openZip(const QString &PathToZip);
    void openArchiveExplorer(const QString &file);
    void populateTreeView(const QMap<QString, QByteArray> &fileMap);
    void onFileDoubleClicked(const QModelIndex &index);
    QMap<QString, QByteArray> loadArchiveToMemory(struct zip *za);

private:
    QTreeView* treeView;
    QStandardItemModel *model;
    QVBoxLayout *layout;
    struct zip *za;
    QMap<QString, QByteArray> fileMap;
    QTemporaryFile* tempFile;
};


#endif //DETROIT_ARCHIVEEXPLORER_H
