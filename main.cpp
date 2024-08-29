#include <QApplication>
#include "ui/MainForm.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QThread logicThread;

    MainLogic::MainFormLogic logic;
    logic.moveToThread(&logicThread);
    logicThread.start();

    MainWindow::MainForm form(nullptr, &logic);

    form.SetupConnect();
    form.InitUI();
    form.show();

    QObject::connect(&app, &QApplication::aboutToQuit, &logicThread, &QThread::quit);
    QObject::connect(&logicThread, &QThread::finished, &logicThread, &QThread::deleteLater);

    return QApplication::exec();
}

