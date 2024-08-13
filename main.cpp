#include <QApplication>
#include "ui/MainForm.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainForm form;
    form.SetupConnect();
    form.InitializeUI();
    form.show();

    return QApplication::exec();
}
