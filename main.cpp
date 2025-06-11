#include "func1.h"

#include <QApplication>

#define test1

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef test1
    /// func1,
    Func1 w;
    w.show();
#endif
    return a.exec();
}
