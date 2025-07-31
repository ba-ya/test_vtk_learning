#include "00headers.h"

#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkDiskSource.h>
#include <vtkLineSource.h>
#include <vtkNamedColors.h>
#include <vtkPlaneSource.h>
#include <vtkPointSource.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkTextSource.h>

void SourceObjectsDemo3d::Draw(std::vector<vtkSmartPointer<vtkRenderer>> renders)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    std::vector<vtkSmartPointer<vtkPolyDataAlgorithm>> source_objs;
    // 一个网格化的多边形球
    source_objs.push_back(vtkSmartPointer<vtkSphereSource>::New());
    // 控制 从“北极”到“南极”的分段数（沿球体“垂直方向”）。
    // 相当于纬度方向的切分数(像地球上的纬线), latitude
    static_cast<vtkSphereSource*>(source_objs.back().GetPointer())->SetPhiResolution(10);
    // 控制 沿赤道的分段数（沿球体“水平方向”）。
    // 相当于经度方向的切分数(像地球上的经线), longitude
    static_cast<vtkSphereSource*>(source_objs.back().GetPointer())->SetThetaResolution(21);
    // 纬度像腰带（横向）	Latitude = 横线 = 纬线
    // 经度像拉链（纵向）	Longitude = 竖线 = 经线
    // Lat ~ Flat（平的）	Latitude 是平行赤道的
    // Long ~ Long line（长线）	Longitude 是连接南北的线

    source_objs.push_back(vtkSmartPointer<vtkConeSource>::New());
    static_cast<vtkConeSource*>(source_objs.back().GetPointer())->SetResolution(51);

    source_objs.push_back(vtkSmartPointer<vtkCylinderSource>::New());
    static_cast<vtkCylinderSource*>(source_objs.back().GetPointer())->SetResolution(51);

    source_objs.push_back(vtkSmartPointer<vtkCubeSource>::New());
    source_objs.push_back(vtkSmartPointer<vtkPlaneSource>::New());
    source_objs.push_back(vtkSmartPointer<vtkTextSource>::New());
    static_cast<vtkTextSource*>(source_objs.back().GetPointer())->SetText("HELLO");
    static_cast<vtkTextSource*>(source_objs.back().GetPointer())->BackingOff();

    source_objs.push_back(vtkSmartPointer<vtkPointSource>::New());
    static_cast<vtkPointSource*>(source_objs.back().GetPointer())->SetNumberOfPoints(500);

    source_objs.push_back(vtkSmartPointer<vtkDiskSource>::New());
    // 环分辨率
    static_cast<vtkDiskSource*>(source_objs.back().GetPointer())->SetCircumferentialResolution(10);

    source_objs.push_back(vtkSmartPointer<vtkLineSource>::New());

    std::vector<vtkSmartPointer<vtkActor>> actors;
    std::vector<vtkSmartPointer<vtkActor2D>> text_actors;

    auto text_property = vtkSmartPointer<vtkTextProperty>::New();
    text_property->SetFontSize(16);
    text_property->SetJustificationToCentered();
    text_property->SetColor(colors->GetColor3d("LightGoldenrodYellow").GetData());

    auto back = vtkSmartPointer<vtkProperty>::New();
    back->SetColor(colors->GetColor3d("Tomato").GetData());

    for (int i = 0; i < source_objs.size(); ++i) {
        auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(source_objs[i]->GetOutputPort());

        actors.push_back(vtkSmartPointer<vtkActor>::New());
        actors[i]->SetMapper(mapper);
        actors[i]->GetProperty()->SetColor(colors->GetColor3d("PeachPuff").GetData());
        actors[i]->SetBackfaceProperty(back);

        auto text_mapper = vtkSmartPointer<vtkTextMapper>::New();
        text_mapper->SetTextProperty(text_property);
        text_mapper->SetInput(source_objs[i]->GetClassName());

        text_actors.push_back(vtkSmartPointer<vtkActor2D>::New());
        text_actors[i]->SetMapper(text_mapper);
        text_actors[i]->SetPosition(120, 16);

        renders[i]->AddActor(actors[i]);
        renders[i]->AddActor2D(text_actors[i]);
    }
}
