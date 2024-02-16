#include "TreeWidget.h"

#include <QApplication>

int main(int nArgs, char* args[]) {
    QApplication app(nArgs, args);

    TreeWidget w;
    w.show();
    return app.exec();
}

