#ifndef FUNC1_H
#define FUNC1_H

#include <QWidget>
#include <vtkSmartPointer.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class Func1;
}
QT_END_NAMESPACE

class vtkRenderer;
class QVTKOpenGLNativeWidget;
class vtkRenderWindowInteractor;
class vtkAxisActor2D;
class Func1 : public QWidget
{
    Q_OBJECT

public:
    Func1(QWidget *parent = nullptr);
    ~Func1();

private slots:
    void on_btn_reset_camera_released();

private:
    void init();
    void init_examples();
    void do_something(QString name_class);
    void init_cells();

    void clear();
    void do_render();
    void reset_camera();

private:
    Ui::Func1 *ui;

    QVTKOpenGLNativeWidget *vtk_widget;
    vtkSmartPointer<vtkRenderer> m_render;
};
#endif // FUNC1_H
