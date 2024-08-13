//
// Created by winlogon on 06.08.2024.
//
#include "ui/MainForm.h"

MainForm::MainForm(QWidget *parent) : QMainWindow(parent) {}

void MainForm::SetSizeWindow() {
    this->adjustSize();
    this->setMinimumSize(800, 600);
}

void MainForm::InitializeUI() {
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

void MainForm::InitializeFileSystemView() {
    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setRootPath("");

    fileSystemView = new QTreeView(this);
    fileSystemView->setModel(fileSystemModel);
    fileSystemView->setRootIndex(fileSystemModel->index(""));
    fileSystemView->setColumnWidth(0, 250);
}

void MainForm::InitializePathLineEdit() {
    pathLineEdit = CreateLineEdit("", Path_LineEdit);
    pathLineEdit->setPlaceholderText("Путь к файлу");
}

void MainForm::InitializeButtons() {
    archiveButton = CreateButton("Добавить в архив", ARCHIVE_File);
    extractButton = CreateButton("Извлечь из архива", EXTRACT_File);
}

void MainForm::InitializeToolBar() {
    toolBar = new QToolBar(this);
    addToolBar(toolBar);
    toolBar->addWidget(archiveButton);
    toolBar->addWidget(extractButton);
}

void MainForm::InitializeMenuBar() {
    auto *MenuBar = new QMenuBar(this);
    auto *SettingsMenu = new QMenu("Настройки", this);

    MenuBar->addMenu(SettingsMenu);
    setMenuBar(MenuBar);
}

void MainForm::SetupLayout() {
    auto *MainLayout = new QVBoxLayout(centralWidget);
    MainLayout->addWidget(toolBar);
    MainLayout->addWidget(pathLineEdit);
    MainLayout->addWidget(fileSystemView, 1);
    MainLayout->addStretch();
}

QPushButton* MainForm::CreateButton(const QString &name, ButtonsState button) {
    auto *NewButton = new QPushButton(name, this);
    emit SetState(NewButton,button);
    connect(NewButton, &QPushButton::clicked, this, &MainForm::Clicked);
    return NewButton;
}

QLineEdit *MainForm::CreateLineEdit(const QString &name, ButtonsState button) {
    auto *NewLineEdit = new QLineEdit(name, this);
    emit SetState(NewLineEdit, button);
    connect(NewLineEdit, &QLineEdit::editingFinished, this, &MainForm::Clicked);
    return NewLineEdit;
}

void MainForm::SetupConnect() {
    //из UI в logic
    connect(this, &MainForm::SetState, &logic, &MainFormLogic::ButtonState);
    connect(this, &MainForm::RequestProcessState, &logic, &MainFormLogic::ProcessState);
    connect(this, &MainForm::SendSelectedFile, &logic, &MainFormLogic::ArchiveFile);

    //из logic в UI
    connect(&logic, &MainFormLogic::UpdateFileSystem, this, &MainForm::UpdateFileSystem);
    connect(&logic, &MainFormLogic::ArchiveFileButton, this, &MainForm::FileSelected);
}

void MainForm::Clicked() {
    emit RequestProcessState(sender());
}

void MainForm::UpdateFileSystem() {
    fileSystemModel->setRootPath(pathLineEdit->text());
    fileSystemView->setRootIndex(fileSystemModel->index(pathLineEdit->text()));
}

void MainForm::FileSelected() {
    QModelIndexList selectedIndexes = fileSystemView->selectionModel()->selectedIndexes();

    if (!selectedIndexes.isEmpty()) {
        QModelIndex selectedIndex = selectedIndexes.first();
        QString selectedFilePath = fileSystemModel->filePath(selectedIndex);
        emit SendSelectedFile(selectedFilePath);
        return;
    }
    qDebug()<<"Index is empty";
}


