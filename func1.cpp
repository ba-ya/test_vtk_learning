#include "func1.h"
#include "./ui_func1.h"
#include "00headers.h"

#include <QTimer>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <QDir>

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
    m_render = vtkSmartPointer<vtkRenderer>::New();
    // m_render->SetBackground(1, 1, 1);

    auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(m_render);
    // 禁止交互
    // renderWindow->SetInteractor(nullptr);

    vtk_widget = new QVTKOpenGLNativeWidget(this);
    vtk_widget->setRenderWindow(renderWindow);
    ui->layout->addWidget(vtk_widget);

    init_examples();
}

void Func1::init_cells()
{
    clear();
    ExampleCells2d::Draw(m_render);
    do_render();
}

void Func1::init_examples()
{
    auto &&get_class_name = [this]() {
        QStringList names;
        QString base = QDir(QDir::currentPath().remove("_build_release")).absoluteFilePath("examples");
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
    }
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
    } else {
        qDebug() << name_class << "not achive";
        return;
    }
    reset_camera();
    do_render();
}

void Func1::clear()
{
    m_render->RemoveAllViewProps();
}

void Func1::reset_camera()
{
    auto camera = m_render->GetActiveCamera();
    camera->SetPosition(0, 0, 600);
    camera->SetViewUp(0, 1, 0);
    camera->SetFocalPoint(0, 0, 0);
    m_render->ResetCamera();  // 重置相机以包含整个图形
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

