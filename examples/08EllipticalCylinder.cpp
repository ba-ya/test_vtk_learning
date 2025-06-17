#include "00headers.h"

#include <vtkCellArray.h>
#include <vtkLinearExtrusionFilter.h>
#include <vtkMath.h>
#include <vtkNamedColors.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyLine.h>
#include <vtkProperty.h>

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

}
