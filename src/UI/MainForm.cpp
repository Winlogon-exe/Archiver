//
// Created by winlogon on 06.08.2024.
//
#include "ui/MainForm.h"

namespace MainWindow {

    MainForm::MainForm(QWidget *parent, MainLogic::MainForm *logic) : QMainWindow(parent), logic(logic)
    {}

    void MainForm::InitUI()
    {
        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        SetSizeWindow();
        setIcon();
        InitFileSystemView();
        InitPathLineEdit();
        InitButtons();
        InitToolBar();
        InitMenuBar();
        SetupLayout();
    }

    void MainForm::SetSizeWindow()
    {
        this->adjustSize();
        this->setMinimumSize(800, 600);
    }

    void MainForm::InitFileSystemView()
    {
        fileSystemModel = new QFileSystemModel(this);
        fileSystemModel->setRootPath("");

        listView = new QListView(this);
        listView->setModel(fileSystemModel);
        listView->setRootIndex(fileSystemModel->index(""));
        connect(listView, &QTreeView::doubleClicked, this, &MainForm::FileDoubleClicked);
    }

    void MainForm::InitPathLineEdit()
    {
        pathLineEdit = CreateLineEdit("", MainLogic::PATH);
        pathLineEdit->setPlaceholderText("Путь к файлу");
    }

    void MainForm::InitButtons()
    {
        archiveButton = CreateButton("Добавить в архив", MainLogic::ARCHIVE);
        extractButton = CreateButton("Извлечь из архива", MainLogic::UNARCHIVE);
        backButton = CreateButton("↑", MainLogic::BACK);
    }

    void MainForm::InitToolBar()
    {
        toolBar = new QToolBar(this);
        addToolBar(toolBar);
        toolBar->addWidget(archiveButton);
        toolBar->addWidget(extractButton);
    }

    void MainForm::InitMenuBar()
    {
        auto *menuBar = new QMenuBar(this);
        auto *settingsMenu = new QMenu("Настройки", this);

        menuBar->addMenu(settingsMenu);
        setMenuBar(menuBar);
    }

    void MainForm::SetupLayout()
    {
        auto *mainLayout = new QVBoxLayout(centralWidget);
        auto *topLayout = new QHBoxLayout();

        topLayout->addWidget(backButton);
        topLayout->addWidget(pathLineEdit);

        mainLayout->addWidget(toolBar);
        mainLayout->addLayout(topLayout);
        mainLayout->addWidget(listView, 1);
        mainLayout->addStretch();
    }

    QPushButton *MainForm::CreateButton(const QString &name, MainLogic::ButtonsState button)
    {
        auto *newButton = new QPushButton(name, this);
        emit SetState(newButton, button);
        connect(newButton, &QPushButton::clicked, this, &MainForm::Clicked);
        return newButton;
    }

    QLineEdit *MainForm::CreateLineEdit(const QString &name, MainLogic::ButtonsState button)
    {
        auto *newLineEdit = new QLineEdit(name, this);
        emit SetState(newLineEdit, button);
        connect(newLineEdit, &QLineEdit::editingFinished, this, &MainForm::Clicked);
        return newLineEdit;
    }

    void MainForm::SetupConnect()
    {
        //из UI в logic
        connect(this, &MainForm::SetState, logic, &MainLogic::MainForm::ButtonState);
        connect(this, &MainForm::RequestProcessState, logic, &MainLogic::MainForm::ProcessState);
        connect(this, &MainForm::SendSelectedFileArchive, logic, &MainLogic::MainForm::Archive);
        connect(this, &MainForm::SendSelectedFileUnArchive, logic, &MainLogic::MainForm::UnArchive);
        connect(this, &MainForm::OpenArchive, logic, &MainLogic::MainForm::FileDoubleClicked);

        //из logic в UI
        connect(logic, &MainLogic::MainForm::UpdateFileSystem, this, &MainForm::UpdateFileSystemForLineEdit);
        connect(logic, &MainLogic::MainForm::ArchiveFileButton, this, &MainForm::FileSelectedForArchive);
        connect(logic, &MainLogic::MainForm::UnArchiveFileButton, this, &MainForm::FileSelectedForUnArchive);
        connect(logic, &MainLogic::MainForm::BackFileSystem, this, &MainForm::BackFileSystem);
        connect(logic, &MainLogic::MainForm::UpdateListView, this, &MainForm::SetListView);
        connect(logic, &MainLogic::MainForm::OpenArchiveInExplorer, this, &MainForm::OpenFilesInArchive);
    }

    void MainForm::Clicked()
    {
        emit RequestProcessState(sender());
    }

    void MainForm::UpdateFileSystemForLineEdit()
    {
        fileSystemModel->setRootPath(pathLineEdit->text());
        listView->setRootIndex(fileSystemModel->index(pathLineEdit->text()));
    }

    void MainForm::FileSelectedForArchive()
    {
        QModelIndexList selectedIndexes = listView->selectionModel()->selectedIndexes();

        if (selectedIndexes.isEmpty())
        {
            qDebug() << "Index is empty";
            return;
        }

        QModelIndex selectedIndex = selectedIndexes.first();
        QString selectedFilePath = fileSystemModel->filePath(selectedIndex);
        emit SendSelectedFileArchive(selectedFilePath);
    }

    void MainForm::FileSelectedForUnArchive()
    {
        QModelIndexList selectedIndexes = listView->selectionModel()->selectedIndexes();

        if (selectedIndexes.isEmpty())
        {
            qDebug() << "Index is empty";
            return;
        }

        QModelIndex selectedIndex = selectedIndexes.first();
        QString selectedFilePath = fileSystemModel->filePath(selectedIndex);

        if (!selectedFilePath.endsWith(".zip") && !selectedFilePath.endsWith(".rar"))
        {

            showMessageBox(this, "Ошибка", "Файл не является архивом ", Critical);
            return;
        }
        emit SendSelectedFileUnArchive(selectedFilePath);
    }

    void MainForm::BackFileSystem()
    {
        QModelIndex currentIndex = listView->rootIndex();
        QString currentPath = fileSystemModel->filePath(currentIndex);
        QString parentPath = QFileInfo(currentPath).absolutePath();

        if (parentPath != currentPath)
        {
            listView->setRootIndex(fileSystemModel->index(parentPath));
        }
        else {
            listView->setRootIndex(fileSystemModel->index(""));
        }
    }

    void MainForm::FileDoubleClicked(const QModelIndex &index)
    {
        emit OpenArchive(index,fileSystemModel);
    }

    void MainForm::SetListView(const QModelIndex &index)
    {
        listView->setRootIndex(index);
    }

    void MainForm::OpenFilesInArchive(const QString &path)
    {
        ArchiveExplorer *explorer = new ArchiveExplorer(nullptr);
        explorer->openArchiveExplorer(path);
        explorer->show();
        QObject::connect(explorer, &QWidget::destroyed, explorer, &QObject::deleteLater);
    }

    void MainForm::contextMenuEvent(QContextMenuEvent *event)
    {
        QMenu contextMenu(this);

        QAction *archiveAction = contextMenu.addAction("Добавить в архив");
        QAction *extractAction = contextMenu.addAction("Извлечь из архива");
        QAction *backAction = contextMenu.addAction("Назад");

        connect(archiveAction, &QAction::triggered, this, &MainForm::FileSelectedForArchive);
        connect(extractAction, &QAction::triggered, this, &MainForm::FileSelectedForUnArchive);
        connect(backAction, &QAction::triggered, this, &MainForm::BackFileSystem);

        contextMenu.exec(event->globalPos());
    }

    bool MainForm::showMessageBox(QWidget* parent, const QString& title, const QString& message, MessageBoxType type)
    {
        QMessageBox msgBox(parent);
        msgBox.setWindowTitle(title);
        msgBox.setText(message);

        switch (type)
        {
            case Info:
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setStandardButtons(QMessageBox::Ok);
                break;
            case Warning:
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setStandardButtons(QMessageBox::Ok);
                break;
            case Critical:
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setStandardButtons(QMessageBox::Ok);
                break;
            case Question:
                msgBox.setIcon(QMessageBox::Question);
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                break;
        }

        return msgBox.exec() == QMessageBox::Yes;
    }

    void MainForm::keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Delete)
        {
            QModelIndexList selectedIndexes = listView->selectionModel()->selectedIndexes();

            if (!selectedIndexes.isEmpty())
            {
                QString selectedFilePath = fileSystemModel->filePath(selectedIndexes.first());
                QFileInfo fileInfo(selectedFilePath);

                if (!showMessageBox(this, "Подтверждение удаления", "Вы действительно хотите удалить " + selectedFilePath + "?", Question))
                {
                    return;
                }

                if (fileInfo.isFile())
                {
                    QFile::remove(selectedFilePath);
                    qDebug() << "File remove:" << selectedFilePath;
                }
                else if (fileInfo.isDir())
                {
                    QDir dir(selectedFilePath);
                    dir.removeRecursively();
                    qDebug() << "Dir remove:" << selectedFilePath;
                }
            }
        }
        else
        {
            QMainWindow::keyPressEvent(event);
        }
    }

    void MainForm::setIcon()
    {
        this->setWindowIcon(QIcon("../icons/app_icon.ico"));
    }
}

