//
// Created by winlogon on 06.08.2024.
//
#include "ui/MainForm.h"

namespace MainWindow {

    MainForm::MainForm(QWidget *parent, MainLogic::MainFormLogic *logic) : QMainWindow(parent), logic(logic)
    {

    }

    void MainForm::SetSizeWindow()
    {
        this->adjustSize();
        this->setMinimumSize(800, 600);
    }

    void MainForm::InitializeUI()
    {
        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        SetSizeWindow();
        InitializeFileSystemView();
        InitializePathLineEdit();
        InitializeButtons();
        InitializeToolBar();
        InitializeMenuBar();
        SetupLayout();
    }

    void MainForm::InitializeFileSystemView()
    {
        fileSystemModel = new QFileSystemModel(this);
        fileSystemModel->setRootPath("");

        fileSystemView = new QTreeView(this);
        fileSystemView->setModel(fileSystemModel);
        fileSystemView->setRootIndex(fileSystemModel->index(""));
        fileSystemView->setColumnWidth(0, 250);
    }

    void MainForm::InitializePathLineEdit()
    {
        pathLineEdit = CreateLineEdit("", MainLogic::Path_LineEdit);
        pathLineEdit->setPlaceholderText("Путь к файлу");
    }

    void MainForm::InitializeButtons()
    {
        archiveButton = CreateButton("Добавить в архив", MainLogic::ARCHIVE_File);
        extractButton = CreateButton("Извлечь из архива", MainLogic::UNARCHIVE_File);
    }

    void MainForm::InitializeToolBar()
    {
        toolBar = new QToolBar(this);
        addToolBar(toolBar);
        toolBar->addWidget(archiveButton);
        toolBar->addWidget(extractButton);
    }

    void MainForm::InitializeMenuBar()
    {
        auto *MenuBar = new QMenuBar(this);
        auto *SettingsMenu = new QMenu("Настройки", this);

        MenuBar->addMenu(SettingsMenu);
        setMenuBar(MenuBar);
    }

    void MainForm::SetupLayout()
    {
        auto *MainLayout = new QVBoxLayout(centralWidget);
        MainLayout->addWidget(toolBar);
        MainLayout->addWidget(pathLineEdit);
        MainLayout->addWidget(fileSystemView, 1);
        MainLayout->addStretch();
    }

    QPushButton *MainForm::CreateButton(const QString &name, MainLogic::ButtonsState button)
    {
        auto *NewButton = new QPushButton(name, this);
        emit SetState(NewButton, button);
        connect(NewButton, &QPushButton::clicked, this, &MainForm::Clicked);
        return NewButton;
    }

    QLineEdit *MainForm::CreateLineEdit(const QString &name, MainLogic::ButtonsState button)
    {
        auto *NewLineEdit = new QLineEdit(name, this);
        emit SetState(NewLineEdit, button);
        connect(NewLineEdit, &QLineEdit::editingFinished, this, &MainForm::Clicked);
        return NewLineEdit;
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

        if(!selectedFilePath.endsWith(".zip")) {
            qDebug() << "File is not zip";
            return;
        }
        emit SendSelectedFileUnArchive(selectedFilePath);
    }
}

