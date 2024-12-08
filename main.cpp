#include <QApplication>
#include <memory>
#include "ui/MainForm.h"
#include "logic/Register.h"
#include "inc/UI/ArchiveExplorer.h"

// zip не открывается в проводнике✅
// нет окна выбора файлов при архивации❌
// не все файлы архивируются(что то с именами)❌
// нет видимости что если файл занят другим процессом его нельзя удалить❌
// нет прогресса/времени архивации❌
// нет названия/иконки✅
// регистрация в реестре✅

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
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if (argc > 1)
    {
        QString filePath = argv[1];
        ArchiveExplorer *explorer = new ArchiveExplorer(nullptr);
        explorer->openArchiveExplorer(filePath);
        explorer->show();
        QObject::connect(explorer, &QWidget::destroyed, explorer, &QObject::deleteLater);
        return app.exec();
    }
    else
    {
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

