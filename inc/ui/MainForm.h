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
#include <QThread>

#include "logic/MainFormLogic.h"
#include "ui/ArchiveExplorer.h"

namespace MainWindow
{
    class MainForm : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainForm(QWidget *parent = nullptr, MainLogic::MainFormLogic *logic = nullptr);

        ~MainForm() = default;

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
        QPushButton *CreateButton(const QString &name, MainLogic::ButtonsState button);
        QLineEdit *CreateLineEdit(const QString &name, MainLogic::ButtonsState button);

    signals:
        void SetState(QObject *sender, MainLogic::ButtonsState state);
        void RequestProcessState(QObject *sender);
        void SendSelectedFileArchive(const QString &file);
        void SendSelectedFileUnArchive(const QString &file);

    public slots:
        void UpdateFileSystem();
        void FileSelectedForArchive();
        void FileSelectedForUnArchive();
        void FileDoubleClicked(const QModelIndex &index);
        void Clicked();

    private:
        QPushButton *archiveButton;
        QPushButton *extractButton;

    private:
        QFileSystemModel *fileSystemModel;
        QLineEdit *pathLineEdit;
        QTreeView *fileSystemView;
        QToolBar *toolBar;
        QWidget *centralWidget;
        MainLogic::MainFormLogic* logic;
    };
}
#endif //DETROIT_MAINFORM_H

