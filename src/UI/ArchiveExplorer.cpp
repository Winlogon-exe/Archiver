#include "ui/ArchiveExplorer.h"

ArchiveExplorer::ArchiveExplorer(QWidget *parent): QWidget(parent)
{}

void ArchiveExplorer::openArchiveExplorer(const QString &PathToZip)
{
    InitUI();
    openZip(PathToZip);
}

void ArchiveExplorer::InitUI()
{
    this->adjustSize();
    this->setMinimumSize(800, 600);

    layout = new QVBoxLayout(this);
    setLayout(layout);

    treeView = new QTreeView(this);
    treeView->setHeaderHidden(false);
    layout->addWidget(treeView);

    model = new QStandardItemModel(treeView);
    model->setHorizontalHeaderLabels({"File Name"});
    treeView->setModel(model);
    connect(treeView, &QTreeView::doubleClicked, this, &ArchiveExplorer::onFileDoubleClicked);

}

void ArchiveExplorer::openZip(const QString &PathToZip)
{
    int err = 0;
    za = zip_open(PathToZip.toUtf8().data(), ZIP_RDONLY, &err);
    if (!za)
    {
        qWarning() << "Failed to open archive:" << PathToZip;
        return;
    }

    fileMap = loadArchiveToMemory(za);
    populateTreeView(fileMap);
}

QMap<QString, QByteArray> ArchiveExplorer::loadArchiveToMemory(struct zip *za)
{
    QMap<QString, QByteArray> fileMaps;
    zip_int64_t numFiles = zip_get_num_entries(za, 0);
    for (zip_int64_t i = 0; i < numFiles; ++i)
    {
        const char *fileName = zip_get_name(za, i, 0);
        QString path(fileName);

        struct zip_file *zf = zip_fopen_index(za,i,0);
        QByteArray fileData;

        char buffer[1024];
        zip_int64_t bytesRead;
        while((bytesRead = zip_fread(zf,buffer, sizeof(bytesRead))) > 0)
        {
            fileData.append(buffer,bytesRead);
        }
        zip_fclose(zf);
        fileMaps.insert(path,fileData);
    }
    zip_close(za);
    return fileMaps;
}

void ArchiveExplorer::populateTreeView(const QMap<QString, QByteArray> &fileMap)
{
    for (const QString &path : fileMap.keys())
    {
        QStringList pathParts = path.split('/');

        QStandardItem *parentItem = model->invisibleRootItem();
        for (const QString &part : pathParts)
        {
            QStandardItem *item = nullptr;

            bool found = false;
            for (int i = 0; i < parentItem->rowCount(); ++i)
            {
                QStandardItem *child = parentItem->child(i);
                if (child->text() == part) {
                    item = child;
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                item = new QStandardItem(part);
                parentItem->appendRow(item);
            }

            parentItem = item;
        }

        QStandardItem *lastItem = parentItem;
        lastItem->setData(path, Qt::UserRole);  // Хранение пути файла
    }
}

void ArchiveExplorer::onFileDoubleClicked(const QModelIndex &index)
{
    QString filePath = index.data(Qt::UserRole).toString();
    qDebug() << "Selected file path:" << filePath;

    QByteArray fileData = fileMap.value(filePath);
    if (fileData.isEmpty()) {
        qWarning() << "File data is empty!";
        return;
    }

    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    QString fileExtension = fileInfo.suffix(); //  расширение файла

    tempFile = new QTemporaryFile();
    QString tempFileName = QDir::temp().filePath(fileName);
    tempFile->setFileName(tempFileName);

    if (!tempFile->open()) {
        qWarning() << "Failed to create temporary file!";
        return;
    }
    qDebug() << "Temporary file created at:" << tempFile->fileName();

    tempFile->write(fileData);
    tempFile->flush();
    qDebug() << "Data written to temporary file.";

    tempFile->close();
    qDebug() << "Temporary file closed.";

    QString tempFilePath = tempFile->fileName();
    qDebug() << "Path to temporary file:" << tempFilePath;

    QDesktopServices::openUrl(QUrl::fromLocalFile(tempFilePath));
    qDebug() << "Opening file with default application.";

}

ArchiveExplorer::~ArchiveExplorer()
{

}

