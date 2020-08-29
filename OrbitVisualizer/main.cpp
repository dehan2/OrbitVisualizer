#include "OrbitVisualizer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OrbitVisualizer w;
    w.show();
    return a.exec();
}
