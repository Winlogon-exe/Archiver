//
// Created by winlogon on 19.08.2024.
//
#include "ui/ArchiveExplorer.h"

ArchiveExplorer::ArchiveExplorer(QWidget *parent)
{}

void ArchiveExplorer::OpenArchiveExplorer(const QString &file)
{
    SetupTreeView(file);
    SetupLayout();
}

void ArchiveExplorer::SetupTreeView(const QString &file)
{
    QList<QString> fileList = getArchiveFileList(file);
    treeView = new QTreeView(this);
    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeView->setDragEnabled(true);
    treeView->setAcceptDrops(true);
    treeView->setAcceptDrops(QAbstractItemView::DragOnly);

    model = new QStandardItemModel;

    for(const QString &fileName : fileList) {
        item = new QStandardItem(fileName);
        model->appendRow(item);
    }

    treeView->setModel(model);
}

void ArchiveExplorer::SetupLayout()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(treeView);

    SetupConnect();
    setLayout(layout);
    this->show();
}

QList<QString> ArchiveExplorer::getArchiveFileList(const QString &archivePath)
{
    QList<QString> fileList;

    int err = 0;
    zip *archive = zip_open(archivePath.toStdString().c_str(), 0, &err);

    if (archive == nullptr) {
        qDebug() << "Failed to open archive: " << archivePath << " Error code: " << err;
        return fileList;
    }

    zip_int64_t numFiles = zip_get_num_entries(archive, 0);

    for (zip_int64_t i = 0; i < numFiles; i++) {
        const char *name = zip_get_name(archive, i, 0);
        if (name != nullptr) {
            fileList.append(QString::fromUtf8(name));
        }
    }

    zip_close(archive);
    return fileList;
}

void ArchiveExplorer::SetupConnect()
{
    connect(treeView, &QTreeView::doubleClicked, this, &ArchiveExplorer::OpenFileInArchive);
}

void ArchiveExplorer::OpenFileInArchive(const QModelIndex &index)
{

}

ArchiveExplorer::~ArchiveExplorer()
{

}
