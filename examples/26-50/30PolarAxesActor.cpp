#include "00headers.h"

#include <vtkNamedColors.h>
#include <vtkParametricEnneper.h>
#include <vtkParametricFunctionSource.h>
#include <vtkPolarAxesActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

void PolarAxesActor2d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto src = vtkSmartPointer<vtkParametricEnneper>::New();
    auto fn_src = vtkSmartPointer<vtkParametricFunctionSource>::New();
    fn_src->SetParametricFunction(src);
    // source类尽量Update一下,只有部分函数会自动渲染管道
    fn_src->Update();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(fn_src->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("CornflowerBlue").GetData());;

    double bounds[6];
    fn_src->GetOutput()->GetBounds(bounds);
    auto xmin = bounds[0]; auto xmax = bounds[1];
    auto ymin = bounds[2]; auto ymax = bounds[3];
    auto zmin = bounds[4]; auto zmax = bounds[5];

    // 最大半径
    std::vector<double> p1{xmin, ymin};
    std::vector<double> p2{xmax, ymax};
    // 方法一
    // 第一个序列起点, 第一个序列终点, 第二个序列起点(默认两个序列等长), 初始值
    // rst = init + Σ(first1[i] * first2[i])
    // auto radius = std::inner_product(std::cbegin(p1), std::cend(p1),
    //                                  std::begin(p2), 0.0);
    // radius = std::sqrt(std::abs(radius));
    // 方法二
    auto dx = std::max(std::abs(xmin), std::abs(xmax));
    auto dy = std::max(std::abs(ymin), std::abs(ymax));
    auto radius = std::sqrt(dx * dx + dy * dy);
    // 极坐标,
    std::vector<double> pole{0, 0, zmax};

    auto polar_actor = vtkSmartPointer<vtkPolarAxesActor>::New();
    // need camera
    polar_actor->SetCamera(renderer->GetActiveCamera());
    // prop
    polar_actor->SetPole(pole.data());
    polar_actor->SetMaximumRadius(radius);
    polar_actor->SetRequestedNumberOfPolarAxes(5);
    polar_actor->SetMaximumAngle(135);
    polar_actor->SetMinimumAngle(-45);

    auto axes_prop = polar_actor->GetPolarAxisProperty();
    auto arcs_prop = polar_actor->GetPolarArcsProperty();
    axes_prop->SetColor(colors->GetColor3d("Red").GetData());
    arcs_prop->SetColor(colors->GetColor3d("Yellow").GetData());

    polar_actor->SetLastRadialAxisProperty(axes_prop);
    polar_actor->SetSecondaryRadialAxesProperty(axes_prop);
    polar_actor->SetSecondaryPolarArcsProperty(arcs_prop);

    renderer->AddActor(actor);
    renderer->AddActor(polar_actor);
}
