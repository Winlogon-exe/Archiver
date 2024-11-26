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
#include <QListView>
#include <QMenuBar>
#include <QTreeView>
#include <QContextMenuEvent>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QThread>


#include "logic/MainForm.h"

namespace MainWindow
{
    enum MessageBoxType {
        Info,
        Warning,
        Critical,
        Question
    };

    class MainForm : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainForm(QWidget *parent = nullptr, MainLogic::MainForm *logic = nullptr);
        ~MainForm() = default;

    protected:
        void contextMenuEvent(QContextMenuEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;

    public:
        void InitUI();
        void InitFileSystemView();
        void InitPathLineEdit();
        void InitButtons();
        void InitToolBar();
        void InitMenuBar();
        void SetupLayout();
        void SetupConnect();
        void SetSizeWindow();
        void setIcon();
        QPushButton *CreateButton(const QString &name, MainLogic::ButtonsState button);
        QLineEdit *CreateLineEdit(const QString &name, MainLogic::ButtonsState button);

    signals:
        void SetState(QObject *sender, MainLogic::ButtonsState state);
        void RequestProcessState(QObject *sender);
        void SendSelectedFileArchive(const QString &file);
        void SendSelectedFileUnArchive(const QString &file);
        void OpenArchive(const QModelIndex &index, QFileSystemModel *model);

    public slots:
        void UpdateFileSystemForLineEdit();
        void BackFileSystem();
        void FileSelectedForArchive();
        void FileSelectedForUnArchive();
        void Clicked();
        void FileDoubleClicked(const QModelIndex &index);
        void SetListView(const QModelIndex &index);
        void OpenFilesInArchive(const QModelIndex &index,const QString &path);
        bool showMessageBox(QWidget* parent, const QString& title, const QString& message, MessageBoxType type);

    private:
        QPushButton *archiveButton;
        QPushButton *extractButton;
        QPushButton *backButton;

    private:
        QFileSystemModel *fileSystemModel;
        QLineEdit *pathLineEdit;
        QListView *listView;
        QToolBar *toolBar;
        QWidget *centralWidget;

    private:
        MainLogic::MainForm* logic;
    };
}
#endif //DETROIT_MAINFORM_H

