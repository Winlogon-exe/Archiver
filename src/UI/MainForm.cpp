//
// Created by winlogon on 06.08.2024.
//
#include "ui/MainForm.h"

namespace MainWindow {

    MainForm::MainForm(QWidget *parent, MainLogic::MainFormLogic *logic) : QMainWindow(parent), logic(logic)
    {}

    void MainForm::SetSizeWindow()
    {
        this->adjustSize();
        this->setMinimumSize(800, 600);
    }

    void MainForm::InitUI()
    {
        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        SetSizeWindow();
        InitFileSystemView();
        InitPathLineEdit();
        InitButtons();
        InitToolBar();
        InitMenuBar();
        SetupLayout();
    }

    void MainForm::InitFileSystemView()
    {
        fileSystemModel = new QFileSystemModel(this);
        fileSystemModel->setRootPath("");

        fileSystemView = new QTreeView(this);
        fileSystemView->setModel(fileSystemModel);
        fileSystemView->setRootIndex(fileSystemModel->index(""));
        fileSystemView->setColumnWidth(0, 250);

        connect(fileSystemView, &QTreeView::doubleClicked, this, &MainForm::FileDoubleClicked);
    }

    void MainForm::InitPathLineEdit()
    {
        pathLineEdit = CreateLineEdit("", MainLogic::Path_LineEdit);
        pathLineEdit->setPlaceholderText("Путь к файлу");
    }

    void MainForm::InitButtons()
    {
        archiveButton = CreateButton("Добавить в архив", MainLogic::ARCHIVE_File);
        extractButton = CreateButton("Извлечь из архива", MainLogic::UNARCHIVE_File);
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
        mainLayout->addWidget(toolBar);
        mainLayout->addWidget(pathLineEdit);
        mainLayout->addWidget(fileSystemView, 1);
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
        connect(this, &MainForm::SetState, logic, &MainLogic::MainFormLogic::ButtonState);
        connect(this, &MainForm::RequestProcessState, logic, &MainLogic::MainFormLogic::ProcessState);
        connect(this, &MainForm::SendSelectedFileArchive, logic, &MainLogic::MainFormLogic::Archive);
        connect(this, &MainForm::SendSelectedFileUnArchive, logic, &MainLogic::MainFormLogic::UnArchive);

        //из logic в UI
        connect(logic, &MainLogic::MainFormLogic::UpdateFileSystem, this, &MainForm::UpdateFileSystem);
        connect(logic, &MainLogic::MainFormLogic::ArchiveFileButton, this, &MainForm::FileSelectedForArchive);
        connect(logic, &MainLogic::MainFormLogic::UnarchiveFileButton, this, &MainForm::FileSelectedForUnArchive);
    }

    void MainForm::Clicked()
    {
        emit RequestProcessState(sender());
    }

    void MainForm::UpdateFileSystem()
    {
        fileSystemModel->setRootPath(pathLineEdit->text());
        fileSystemView->setRootIndex(fileSystemModel->index(pathLineEdit->text()));
    }

    void MainForm::FileSelectedForArchive()
    {
        QModelIndexList selectedIndexes = fileSystemView->selectionModel()->selectedIndexes();

        if (selectedIndexes.isEmpty()) {
            qDebug() << "Index is empty";
            return;
        }

        QModelIndex selectedIndex = selectedIndexes.first();
        QString selectedFilePath = fileSystemModel->filePath(selectedIndex);
        emit SendSelectedFileArchive(selectedFilePath);
    }

    void MainForm::FileSelectedForUnArchive() {
        QModelIndexList selectedIndexes = fileSystemView->selectionModel()->selectedIndexes();

        if (selectedIndexes.isEmpty()) {
            qDebug() << "Index is empty";
            return;
        }

        QModelIndex selectedIndex = selectedIndexes.first();
        QString selectedFilePath = fileSystemModel->filePath(selectedIndex);

        if(!selectedFilePath.endsWith(".zip") && !selectedFilePath.endsWith(".rar")) {
            qDebug() << "File is not archive";
            return;
        }
        emit SendSelectedFileUnArchive(selectedFilePath);
    }

    void MainForm::FileDoubleClicked(const QModelIndex &index) {
        archiveExplorer = std::make_shared<ArchiveExplorer>();
        connect(this, &MainForm::OpenArchiveExplorer, archiveExplorer.get(), &ArchiveExplorer::OpenArchiveExplorer);
        connect(this, &MainForm::OpenFile, archiveExplorer.get(), &ArchiveExplorer::OpenFileInExplorer);
        connect(this, &MainForm::OpenDir, archiveExplorer.get(), &ArchiveExplorer::OpenDirInExplorer);

        QString selectedFilePath = fileSystemModel->filePath(index);
        qDebug()<<selectedFilePath;

        //рефактор проверок что это архив
        if (selectedFilePath.endsWith(".zip") || selectedFilePath.endsWith(".rar")) {
            emit OpenArchiveExplorer(selectedFilePath);
            qDebug()<<"is a Archive";
        }
        else if (QFileInfo(selectedFilePath).isFile()) {
            emit OpenFile(selectedFilePath);
            qDebug()<<"is a File";
        }
        else if (QFileInfo(selectedFilePath).isDir()) {
            emit OpenDir(selectedFilePath);
            qDebug()<<"is a Dir";
        }
    }
}

