//
// Created by winlogon on 06.08.2024.
//

#ifndef DETROIT_MAINFORM_H
#define DETROIT_MAINFORM_H

#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <QWidget>
#include <QToolBar>
#include <QMenuBar>
#include <QTreeView>
#include <QFileSystemModel>
#include <QLineEdit>
#include "logic/MainFormLogic.h"

class MainForm : public QMainWindow
{
    Q_OBJECT
public:
     explicit MainForm(QWidget *parent = nullptr);
    ~MainForm() override = default;

public:
    void InitializeUI();
    void InitializeFileSystemView();
    void InitializePathLineEdit();
    void InitializeButtons();
    void InitializeToolBar();
    void InitializeMenuBar();

    void SetupLayout();
    void SetupConnect();
    void SetSizeWindow();
    QPushButton* CreateButton(const QString& name, ButtonsState button);
    QLineEdit* CreateLineEdit(const QString& name, ButtonsState button);

signals:
    void SetState(QObject* sender, ButtonsState state);
    void RequestProcessState(QObject* sender);
    void SendSelectedFile(const QString& file);

public slots:
    void Clicked();
    void UpdateFileSystem();
    void FileSelected();

private:
    QPushButton *archiveButton;
    QPushButton *extractButton;

private:
    QFileSystemModel *fileSystemModel;
    QLineEdit *pathLineEdit;
    QTreeView *fileSystemView;
    QToolBar *toolBar;
    QWidget *centralWidget;
    MainFormLogic logic;
};

#endif //DETROIT_MAINFORM_H
