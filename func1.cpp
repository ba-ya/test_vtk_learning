#include "func1.h"
#include "./ui_func1.h"
#include "01cells.h"

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
    init();
}

Func1::~Func1()
{
    delete ui;
}

void Func1::init()
{
    m_render = vtkSmartPointer<vtkRenderer>::New();
    m_render->SetBackground(1, 1, 1);

    auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(m_render);
    // 禁止交互
    // renderWindow->SetInteractor(nullptr);

    vtk_widget = new QVTKOpenGLNativeWidget(this);
    vtk_widget->setRenderWindow(renderWindow);
    ui->layout->addWidget(vtk_widget);

    // QTimer::singleShot(10, this, [this]() {
    //     init_cells();
    // });

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
    auto &&init_table = [](QTableWidget *table, QString dir, QStringList files) {
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        auto cnt_row = files.size();
        table->setRowCount(cnt_row);
        table->setColumnCount(1);
        for (int i = 0; i < cnt_row; ++i) {
            auto file_path = QDir(dir).absoluteFilePath(files.at(i));

            QFile file(file_path);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << file_path << "open fail";
                return;
            }
            QTextStream in(&file);
            QString content = in.readAll();
            file.close();

            QString item_str = files.at(i);
            // 使用正则匹配 namespace 名称
            QRegularExpression regex(R"(namespace\s*(\w+)\s*\{)");
            QRegularExpressionMatch match = regex.match(content);
            if (match.hasMatch()) {
                item_str += QString("(%1)").arg(match.captured(1));
            }

            table->setItem(i, 0, new QTableWidgetItem(item_str));
            qDebug().noquote() << item_str;
        }
    };
    QStringList filters = { "*.h"};
    QString base = QDir(QDir::currentPath().remove("_build_release")).absoluteFilePath("examples");
    auto list_ex = QDir(base).entryList(filters, QDir::Files, QDir::Name);
    init_table(ui->table, base, list_ex);

    connect(ui->table, &QTableWidget::itemDoubleClicked, this, [this](QTableWidgetItem *item) {
        QRegularExpression reg("\\((\\w+)\\)");
        QRegularExpressionMatch match = reg.match(item->text().trimmed());
        if (match.hasMatch()) {
            ui->label_name->setText(item->text());
            do_something(match.captured(1));
            return;
        }
        qDebug() << item->text() << "not match";
    });
}

void Func1::do_something(QString name_class)
{
    clear();
    if (name_class == "ExampleCells2d") {
        ExampleCells2d::Draw(m_render);
    } else {
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
    ui->label_name->clear();
    clear();
    do_render();
}

