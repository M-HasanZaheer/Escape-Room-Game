#include <QApplication>
#include "EscapeRoom.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    EscapeRoom window;
    window.show();

    return app.exec();
}
