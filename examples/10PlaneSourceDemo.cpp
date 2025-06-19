#include "00headers.h"

#include <vtkArrowSource.h>
#include <vtkLegendBoxActor.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

enum GeoShpere{
    G_origin,
    G_center,
    G_point1,
    G_point2,

    G_count,
};

enum GeoAxis{
    A_xaxis,
    A_yaxis,
    A_normal,

    A_count,
};

// 和08画箭头是一样的逻辑
namespace {
vtkSmartPointer<vtkPolyData> CreateArrow(double& length,
                                         std::array<double, 3>& startPoint,
                                         std::array<double, 3>& endPoint);
}

void PlaneSourceDemo3d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto plane_source = vtkSmartPointer<vtkPlaneSource>::New();
    plane_source->SetOrigin(0, 0, 0);
    plane_source->SetPoint1(1, 0, 0);
    plane_source->SetPoint2(0, 1, 0);
    plane_source->Update();

    std::array<double, 6> bounds;
    // 手动调GetOutput()必须先Update()
    //  GetBounds(xmin,xmax, ymin,ymax, zmin,zmax).
    plane_source->GetOutput()->GetBounds(bounds.data());
    double length = std::max(bounds[1] - bounds[0], bounds[3] - bounds[2]);

    /// 面
    auto plane_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    plane_mapper->SetInputConnection(plane_source->GetOutputPort());
    auto plane_actor = vtkSmartPointer<vtkActor>::New();
    plane_actor->SetMapper(plane_mapper);

    /// 各种点: 原点, 中心点, 点1, 点2
    std::vector<vtkSmartPointer<vtkActor>> sphere_actors;
    // 数据源
    auto sphere_source = vtkSmartPointer<vtkSphereSource>::New();
    sphere_source->SetRadius(length * .04);
    // 颜色
    std::array<vtkColor3d, G_count> sphere_color;
    sphere_color[G_origin] = colors->GetColor3d("OrangeRed");
    sphere_color[G_center] = colors->GetColor3d("Gold");
    sphere_color[G_point1] = colors->GetColor3d("MediumSpringGreen");
    sphere_color[G_point2] = colors->GetColor3d("Magenta");
    // 位置
    std::array<double*, G_count> sphere_pos;
    sphere_pos[G_origin] = plane_source->GetOrigin();
    sphere_pos[G_center] = plane_source->GetCenter();
    sphere_pos[G_point1] = plane_source->GetPoint1();
    sphere_pos[G_point2] = plane_source->GetPoint2();
    for (int i = 0; i < G_count; ++i) {
        auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(sphere_source->GetOutputPort());
        auto actor = vtkSmartPointer<vtkActor>::New();
        actor->SetPosition(sphere_pos[i]);
        actor->SetMapper(mapper);
        actor->GetProperty()->SetDiffuseColor(sphere_color[i].GetData());

        sphere_actors.push_back(actor);
    }

    /// 各种轴: x轴, y轴, 法线
    std::array<double, 3> origin;
    std::array<double, 3> normal;
    std::array<double, 3> point1;
    std::array<double, 3> point2;
    for (int i = 0; i < 3; i++) {
        origin[i] = plane_source->GetOrigin()[i];
        normal[i] = plane_source->GetNormal()[i] * length;
        point1[i] = plane_source->GetPoint1()[i];
        point2[i] = plane_source->GetPoint2()[i];
    }
    std::vector<vtkSmartPointer<vtkActor>> axis_actors;
    // 数据源
    std::array<std::array<double, 3>, A_count> axis_endpoint;
    axis_endpoint[A_xaxis] = point1;
    axis_endpoint[A_yaxis] = point2;
    axis_endpoint[A_normal] = normal;
    // 颜色
    std::array<vtkColor3d, A_count> axis_color;
    axis_color[A_xaxis] = colors->GetColor3d("lime");
    axis_color[A_yaxis] = colors->GetColor3d("orange");
    axis_color[A_normal] = colors->GetColor3d("OrangeRed");
    for (int i = 0; i < A_count; ++i) {
        auto polydata = CreateArrow(length, origin, axis_endpoint[i]);
        auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(polydata);
        auto actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->SetDiffuseColor(axis_color[i].GetData());

        axis_actors.push_back(actor);
    }

    /// 图示
    auto legend = vtkSmartPointer<vtkLegendBoxActor>::New();
    int cnt_legend = 7;
    legend->SetNumberOfEntries(cnt_legend);
    legend->UseBackgroundOn();
    legend->SetBackgroundColor(colors->GetColor3d("DarkSlateGray").GetData());
    // 设置图例位置, (0,0)窗口左下角, (1,1)窗口右上角
    legend->GetPositionCoordinate()->SetValue(.8, .0);
    // 设置图例长宽
    legend->GetPosition2Coordinate()->SetValue(.2, .3);
    // 图示名字
    QStringList legend_name = {
        "origin",
        "center",
        "point1",
        "point2",
        "xaxis",
        "yaxis",
        "normal",
    };
    // 颜色
    std::array<vtkColor3d, 7> legend_color = {
        sphere_color[G_origin],sphere_color[G_center],sphere_color[G_point1], sphere_color[G_point2],
        axis_color[A_xaxis],axis_color[A_yaxis],axis_color[A_normal]
    };
    // polydata
    auto pd_1 = sphere_source->GetOutput();
    auto pd_2 = CreateArrow(length, origin, point1);
    std::array<vtkPolyData*, 7> legend_pd = {
        pd_1, pd_1, pd_1, pd_1,
        pd_2, pd_2, pd_2
    };
    for (int i = 0; i < cnt_legend; i++) {
        legend->SetEntry(i,
                         legend_pd[i % legend_pd.size()],
                         legend_name[i % legend_name.size()].toUtf8().constData(),
                         legend_color[i % legend_color.size()].GetData());
    }

    renderer->AddActor(plane_actor);
    for (auto &actor : sphere_actors) {
        renderer->AddActor(actor);
    }
    for (auto &actor : axis_actors) {
        renderer->AddActor(actor);
    }
    renderer->AddActor(legend);
}

namespace {
vtkSmartPointer<vtkPolyData> CreateArrow(double& pdlength,
                                         std::array<double, 3>& startPoint,
                                         std::array<double, 3>& endPoint) {
    auto arrow_source = vtkSmartPointer<vtkArrowSource>::New();
    arrow_source->SetShaftRadius(pdlength * .01);
    arrow_source->SetShaftResolution(3);
    arrow_source->SetTipLength(pdlength * .1);
    arrow_source->SetTipRadius(pdlength * .05);
    arrow_source->SetTipResolution(3);

    // 计算basis, 基坐标
    std::array<double, 3> normalize_x;
    std::array<double, 3> normalize_y;
    std::array<double, 3> normalize_z;
    // x 轴
    vtkMath::Subtract(endPoint.data(), startPoint.data(), normalize_x);
    double length = vtkMath::Norm(normalize_x.data());
    vtkMath::Normalize(normalize_x.data());
    // z轴, x cross 随机向量
    auto rng = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();;
    rng->SetSeed(8775070);
    std::array<double, 3> arbitrary;
    for (int i = 0; i < 3; ++i) {
        arbitrary[i] = rng->GetNextRangeValue(-10, 10);
    }
    vtkMath::Cross(normalize_x.data(), arbitrary.data(), normalize_z.data());
    vtkMath::Normalize(normalize_z.data());
    // y轴, z Cross x
    vtkMath::Cross(normalize_z.data(), normalize_x.data(), normalize_y.data());

    // transform
    auto matrix = vtkSmartPointer<vtkMatrix4x4>::New();
    matrix->Identity();
    for (int i = 0; i < 3; ++i) {
        matrix->SetElement(i, 0, normalize_x[i]);
        matrix->SetElement(i, 1, normalize_y[i]);
        matrix->SetElement(i, 2, normalize_z[i]);
    }
    auto transform = vtkSmartPointer<vtkTransform>::New();
#if 0
    // 通常推荐“先局部后世界”, 这种更通用点
    transform->SetMatrix(matrix);
    transform->Scale(length, length, length);
    transform->PostMultiply();
    transform->Translate(startPoint.data());
#else
    transform->Translate(startPoint.data());
    transform->Concatenate(matrix);
    transform->Scale(length, length, length);
#endif

    // filter
    auto filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    filter->SetTransform(transform);
    filter->SetInputConnection(arrow_source->GetOutputPort());
    filter->Update();

    auto polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata = filter->GetOutput();
    return polydata;
}
}
