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
    void InitUI();
    void setupConnect();
    void openArchiveExplorer(const QString &file);

private:
    QWidget *centralWidget;
};


#endif //DETROIT_ARCHIVEEXPLORER_H
