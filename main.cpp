#include <QApplication>
#include "ui/MainForm.h"

// zip не открывается в проводнике
// нет окна выбора файлов при архивации
// не все файлы архивируются(что то с именами)
// нет видимости что если файл занят другим процессом его нельзя удалить
// нет прогресса/времени архивации
// нет названия/иконки
// в windows нет этой проги чтобы "открыть с помощью"

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

