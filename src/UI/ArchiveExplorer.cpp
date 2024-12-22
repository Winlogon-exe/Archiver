#include <QStandardPaths>
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

    loadArchiveToMemory();
    populateTreeView();
}

void ArchiveExplorer::loadArchiveToMemory()
{
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
        fileMap.insert(path,fileData);
    }
    zip_close(za);
}

void ArchiveExplorer::populateTreeView()
{
    QFileIconProvider iconProvider; // Провайдер иконок для файлов и папок

    for (const QString &path : fileMap.keys())
    {
        QStringList pathParts = path.split('/',Qt::SkipEmptyParts);

        QStandardItem *parentItem = model->invisibleRootItem();
        for (const QString &part : pathParts)
        {
            QStandardItem *item;

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

                // Установка иконки
                if (pathParts.last() == part)
                { // Если это последний элемент (файл)
                    QFileInfo fileInfo(part);
                    item->setIcon(iconProvider.icon(fileInfo));
                }
                else 
                {
                    item->setIcon(iconProvider.icon(QFileIconProvider::Folder));
                }

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
    // путь файла из данных модели
    QString filePath = index.data(Qt::UserRole).toString();
    qDebug() << "Selected file path:" << filePath;

    if (tempDir.isEmpty())
    {
        tempDir = QDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation)).filePath("ZipSter");
        QDir().mkpath(tempDir);
        qDebug() << "Temporary directory created:" << tempDir;

        // все файлы из архива
        for (auto it = fileMap.begin(); it != fileMap.end(); ++it)
        {
            QString relativePath = it.key(); // Относительный путь в архиве
            QString fullPath = QDir(tempDir).filePath(relativePath);
            QFileInfo fileInfo(fullPath);

            if (relativePath.endsWith('/')) {
                QDir().mkpath(fullPath);
                qDebug() << "Created directory:" << fullPath;
                continue;
            }

            QDir().mkpath(fileInfo.absolutePath());

            QFile file(fullPath);
            if (file.open(QIODevice::WriteOnly))
            {
                file.write(it.value());
                file.close();
            }
            else
            {
                qWarning() << "Failed to write file:" << fullPath;
            }
        }
    }

    QString tempFilePath = QDir(tempDir).filePath(filePath);
    qDebug() << "Temporary file path for execution:" << tempFilePath;

    QDesktopServices::openUrl(QUrl::fromLocalFile(tempFilePath));
    qDebug() << "Opening file with default application.";
}

void ArchiveExplorer::cleanupTempDir()
{
    if (!tempDir.isEmpty())
    {
        QDir dir(tempDir);
        if (dir.exists())
        {
            dir.removeRecursively();
            qDebug() << "Temporary directory cleaned up:" << tempDir;
        }
    }
}

ArchiveExplorer::~ArchiveExplorer()
{
    cleanupTempDir();
}

