﻿#include "func1.h"
#include "./ui_func1.h"
#include "00headers.h"

#include <QTimer>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <QDir>
#include <vtkNamedColors.h>

Func1::Func1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Func1)
{
    ui->setupUi(this);
    setWindowTitle("vtk examples");
    init();
}

Func1::~Func1()
{
    delete ui;
}

void Func1::init()
{
    auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

    vtk_widget = new QVTKOpenGLNativeWidget(this);
    vtk_widget->setRenderWindow(renderWindow);
    ui->layout->addWidget(vtk_widget);

    init_examples();
}

void Func1::resize_render(int count)
{
    for (auto &render : renders) {
        vtk_widget->renderWindow()->RemoveRenderer(render);
    }
    renders.clear();
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    std::vector<double *> color_list = {
        colors->GetColor3d("SlateGray").GetData(),
        colors->GetColor3d("DarkSlateGray").GetData(),
    };
    for (int i = 0; i < count; ++i) {
        auto render = vtkSmartPointer<vtkRenderer>::New();
        render->SetBackground(color_list.at(i % color_list.size()));

        vtk_widget->renderWindow()->AddRenderer(render);
        renders.push_back(render);
    }
}

void Func1::init_examples()
{
    auto &&get_class_name = [this]() {
        QStringList names;
        auto dir = QDir(QDir::currentPath());
        dir.cdUp(); dir.cdUp();
        QString base = dir.absoluteFilePath("examples");
        auto file_path = QDir(base).absoluteFilePath("00headers.h");
        QFile file(file_path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << file_path << "open fail";
            return names;
        }
        QTextStream in(&file);
        QString content = in.readAll();
        file.close();

        // 使用正则匹配 namespace 名称
        static QRegularExpression regex(R"(namespace\s*(\w+)\s*\{)");
        QRegularExpressionMatchIterator i = regex.globalMatch(content);
        while (i.hasNext()) {
            names << i.next().captured(1);
        }
        return names;
    };

    auto &&init_table = [](QTableWidget *table, QStringList files) {
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        auto cnt_row = files.size();
        table->setRowCount(cnt_row);
        table->setColumnCount(1);
        for (int i = 0; i < cnt_row; ++i) {
            table->setItem(i, 0, new QTableWidgetItem(files.at(i)));
        }
    };
    init_table(ui->table, get_class_name());

    connect(ui->table, &QTableWidget::itemDoubleClicked, this, [this](QTableWidgetItem *item) {
        ui->lineEdit_name->setText(QString("%1, %2").arg(item->row()+ 1).arg(item->text()));
        do_something(item->text());
    });
}

void Func1::do_something(QString name_class)
{
    if (ui->checkBox_clear_before->isChecked()) {
        clear();
        do_render();
    }
    int size = 1;
    if (renders.size() != size) {
        resize_render(size);
    }
    auto m_render = renders[0];
    if (name_class == "ExampleCells2d") {
        ExampleCells2d::Draw(m_render);
    } else if (name_class == "Cylinder3d") {
        Cylinder3d::Draw(m_render);
    } else if (name_class == "SimpleOperations") {
        SimpleOperations::Draw(m_render);
    } else if (name_class == "Axes3d") {
        Axes3d::Draw(m_render);
    } else if (name_class == "ColorLine2d") {
        ColorLine2d::Draw(m_render);
    } else if (name_class == "Cube3d") {
        Cube3d::Draw(m_render);
    } else if (name_class == "Dodecahedron3d") {
        Dodecahedron3d::Draw(m_render);
    } else if (name_class == "EllipticalCylinder3d") {
        EllipticalCylinder3d::Draw(m_render);
    } else if (name_class == "GeometricObjectsDemo3d") {
        resize_render(8);
        GeometricObjectsDemo3d::Draw(renders);
    } else if (name_class == "PlaneSourceDemo3d") {
        PlaneSourceDemo3d::Draw(m_render);
    } else if (name_class == "Planes3d") {
        resize_render(2);
        Planes3d::Draw(renders);
    } else if (name_class == "ShrinkCube3d") {
        ShrinkCube3d::Draw(m_render);
    } else if (name_class == "SourceObjectsDemo3d") {
        resize_render(9);
        SourceObjectsDemo3d::Draw(renders);
    } else if (name_class == "Cell3DDemonstration3d") {
        resize_render(8);
        Cell3DDemonstration3d::Draw(renders);
    } else {
        qDebug() << name_class << "not achive";
        return;
    }
    reset_camera();
    do_render();
}

void Func1::clear()
{
    for (auto render : renders) {
        render->RemoveAllViewProps();
    }
}

void Func1::reset_camera()
{
    for (auto render : renders) {
        auto camera = render->GetActiveCamera();
        camera->SetPosition(0, 0, 600);
        camera->SetViewUp(0, 1, 0);
        camera->SetFocalPoint(0, 0, 0);
        render->ResetCamera();
        render->ResetCameraClippingRange();
    }
}

void Func1::do_render()
{
    vtk_widget->renderWindow()->Render();
}

void Func1::on_btn_reset_camera_released()
{
    reset_camera();
    do_render();
}

void Func1::on_btn_clear_released()
{
    ui->lineEdit_name->clear();
    clear();
    do_render();
}

