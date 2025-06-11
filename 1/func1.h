#ifndef FUNC1_H
#define FUNC1_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Func1;
}
QT_END_NAMESPACE

class Func1 : public QWidget
{
    Q_OBJECT

public:
    Func1(QWidget *parent = nullptr);
    ~Func1();

private:
    Ui::Func1 *ui;
};
#endif // FUNC1_H
