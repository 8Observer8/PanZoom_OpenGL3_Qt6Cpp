#include "Widget.h"

#include <QtGui/QSurfaceFormat>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setSamples(8);

    Widget w;
    w.setFormat(format);
    w.show();
    return a.exec();
}
