#include <QApplication>
#include <memory>

#include "ui/MainForm.h"
#include "logic/Register.h"
#include "inc/UI/ArchiveExplorer.h"

void addToRegister()
{
    Register reg;
    if (!reg.isRegisteredForOpenWith())
    {
        reg.registerAppForOpenWith();
    }
    else
    {
        qDebug() << "Application already registered for 'Open With'.";
    }
    reg.addArchiveOptionToContextMenu();
}

void printArg(int argc,char *argv[])
{
    qDebug() << "Arguments passed to the program:";
    for (int i = 0; i < argc; i++)
    {
        qDebug() << "argv[" << i << "]:" << argv[i];
    }

    qDebug() << "Number of arguments:" << argc;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if (argc > 1)
    {
        QString filePath = argv[1];

        printArg(argc,argv);

        ArchiveExplorer *explorer = new ArchiveExplorer(nullptr);
        explorer->openArchiveExplorer(filePath);
        explorer->show();
        QObject::connect(explorer, &QWidget::destroyed, explorer, &QObject::deleteLater);
        return app.exec();
    }
    else
    {
        printArg(argc,argv);
        addToRegister();

        QThread logicThread;
        MainLogic::MainForm logic;
        logic.moveToThread(&logicThread);
        logicThread.start();

        MainWindow::MainForm form(nullptr, &logic);
        form.SetupConnect();
        form.InitUI();
        form.show();

        QObject::connect(&app, &QApplication::aboutToQuit, &logicThread, &QThread::quit);
        QObject::connect(&logicThread, &QThread::finished, &logicThread, &QThread::deleteLater);
        return app.exec();
    }
}

