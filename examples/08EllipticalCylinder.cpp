#include "00headers.h"

#include <vtkArrowSource.h>
#include <vtkCellArray.h>
#include <vtkLinearExtrusionFilter.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyLine.h>
#include <vtkProperty.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTubeFilter.h>

void EllipticalCylinder3d::Draw(vtkRenderer *renderer)
{
    // r1：x 方向半径（控制宽度）
    // r2：y 方向半径（控制高度）
    // angle：当前的角度（弧度制）
    // center_x, center_y：圆心坐标
    // z = 0.0：所有点都在同一平面上（z=0），即在二维平面上绘制
    double angle = 0;
    double r1 = 50, r2 = 30;
    double center_x = 10.0, center_y = 5.0;

    auto points  = vtkSmartPointer<vtkPoints>::New();
    int id = 0;
    // vtk里面所有角度都是弧度制,angle也应该是弧度
    // 走360度,每隔3度取个点
    while (angle <= 2.0 * vtkMath::Pi() + (vtkMath::Pi() / 60)) {
        points->InsertNextPoint(r1 * cos(angle) + center_x,
                                r2 * sin(angle) + center_y,
                                0.0);
        angle = angle + (vtkMath::Pi() / 60);
        ++id;
    }

    // 线是上面id个点连接而成的
    auto line = vtkSmartPointer<vtkPolyLine>::New();
    line->GetPointIds()->SetNumberOfIds(id);
    for (int i = 0; i < id; ++i) {
        line->GetPointIds()->SetId(i, i);
    }

    auto lines = vtkSmartPointer<vtkCellArray>::New();
    lines->InsertNextCell(line);

    auto polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);
    polydata->SetLines(lines);

    // extrusionfilter
    auto extrude = vtkSmartPointer<vtkLinearExtrusionFilter>::New();
    extrude->SetInputData(polydata);
    extrude->SetExtrusionTypeToNormalExtrusion(); // 使用法线进行拉伸
    extrude->SetVector(0, 0, 100); // 指定 extrusion 方向
    extrude->Update();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    // 圆弧线段
    // mapper->SetInputData(polydata);
    // 拉伸变成圆柱面片
    mapper->SetInputConnection(extrude->GetOutputPort());

    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("Banana").GetData());
    auto back = vtkSmartPointer<vtkProperty>::New();
    back->SetColor(colors->GetColor3d("Tomato").GetData());
    actor->SetBackfaceProperty(back);

    renderer->AddActor(actor);

    /// ----arrow
    double start_point[3], end_point[3];
    start_point[0] = center_x;
    start_point[1] = center_y;
    start_point[2] = 0;
    for (int i = 0; i < 3; ++i) {
        end_point[i] = start_point[i] + extrude->GetVector()[i];
    }

    /// basis, 构造正交坐标系
    double normalize_x[3], normalize_y[3], normalize_z[3];
    /// x 方向,
    vtkMath::Subtract(end_point, start_point, normalize_x);
    double length = vtkMath::Norm(normalize_x);
    vtkMath::Normalize(normalize_x);
    /// z, x cross arbitrary
    // 随机向量,目的是与x不平行(使用随机数,概率很小的)
    auto rng = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
    rng->SetSeed(8775070);
    auto max_r = 10;
    double arbitrary[3];
    for (int i = 0; i < 3; ++i) {
        arbitrary[i] = rng->GetRangeValue(-max_r, max_r);
        rng->Next();
    }
    // 叉乘, 目的是得到与x和随机向量垂直的一个向量z,
    vtkMath::Cross(normalize_x, arbitrary, normalize_z);
    // 归一化
    vtkMath::Normalize(normalize_z);
    /// y, z cross x, cross得到的向量方向由右手定则决定
    vtkMath::Cross(normalize_z, normalize_x, normalize_y);
    vtkMath::Normalize(normalize_y);

    // transform
    auto matrix = vtkSmartPointer<vtkMatrix4x4>::New();
    matrix->Identity();
    for (int i = 0; i < 3; ++i) {
        matrix->SetElement(i, 0, normalize_x[i]);
        matrix->SetElement(i, 1, normalize_y[i]);
        matrix->SetElement(i, 2, normalize_z[i]);
    }
    auto transform = vtkSmartPointer<vtkTransform>::New();
    transform->SetMatrix(matrix);
#if 0
    // transform->PostMultiply();
    // transform->Translate(0, -center_y, center_x);
#else
    transform->Scale(length, length, length); // 局部坐标系(matrix)下缩放
    transform->PostMultiply(); // 之后的操作在世界坐标系下进行
    transform->Translate(start_point);
#endif

    auto arrow_source = vtkSmartPointer<vtkArrowSource>::New();
    // 控制 箭头头部（尖端圆锥体） 周围的分段数。
    arrow_source->SetTipResolution(10);
    // 控制 箭头的杆（柱状部分） 的圆周分段。
    arrow_source->SetShaftResolution(21);

    // filter
    auto transform_pd = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transform_pd->SetTransform(transform);
    transform_pd->SetInputConnection(arrow_source->GetOutputPort());

    auto arrow_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    arrow_mapper->SetInputConnection(transform_pd->GetOutputPort());

    auto arrow_actor = vtkSmartPointer<vtkActor>::New();
    arrow_actor->SetMapper(arrow_mapper);
    arrow_actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());

    renderer->AddActor(arrow_actor);

    // -----蓝圈, 圆弧
    auto tubes = vtkSmartPointer<vtkTubeFilter>::New();
    tubes->SetInputData(polydata);
    tubes->SetRadius(2.0);
    tubes->SetNumberOfSides(21);

    auto line_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    line_mapper->SetInputConnection(tubes->GetOutputPort());

    auto line_actor = vtkSmartPointer<vtkActor>::New();
    line_actor->SetMapper(line_mapper);
    line_actor->GetProperty()->SetColor(colors->GetColor3d("Peacock").GetData());

    renderer->AddActor(line_actor);

}
