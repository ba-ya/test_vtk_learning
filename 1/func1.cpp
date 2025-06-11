#include "func1.h"
#include "./ui_func1.h"

Func1::Func1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Func1)
{
    ui->setupUi(this);
}

Func1::~Func1()
{
    delete ui;
}
