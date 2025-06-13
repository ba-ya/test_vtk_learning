#include "00headers.h"

#include <vtkLine.h>
#include <vtkPolyData.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkNamedColors.h>
#include <vtkUnsignedCharArray.h>
#include <vtkCellData.h>

void ColorLine2d::Draw(vtkRenderer *renderer)
{
    double origin[3] = {0.0, 0.0, 0.0};
    double p0[3] = {1.0, 0.0, 0.0};
    double p1[3] = {0.0, 1.0, 0.0};

    // 点数据
    auto pts = vtkSmartPointer<vtkPoints>::New();
    pts->InsertNextPoint(p0);
    pts->InsertNextPoint(origin);
    pts->InsertNextPoint(p1);

    // 线数据
    auto line0 = vtkSmartPointer<vtkLine>::New();
    line0->GetPointIds()->SetId(0, 0); // line的起点, 使用poly_data的第0个点(origin)
    line0->GetPointIds()->SetId(1, 1); // line的终点, 使用poly_data的第1个点(p0)

    auto line1 = vtkSmartPointer<vtkLine>::New();
    line1->GetPointIds()->SetId(0, 1);
    line1->GetPointIds()->SetId(1, 2);

    auto lines = vtkSmartPointer<vtkCellArray>::New();
    lines->InsertNextCell(line0);
    lines->InsertNextCell(line1);

    // 与polydata链接起来
    auto line_poly_data = vtkSmartPointer<vtkPolyData>::New();
    line_poly_data->SetPoints(pts);
    line_poly_data->SetLines(lines);

    // color
    auto named_colors = vtkSmartPointer<vtkNamedColors>::New();
    auto colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->InsertNextTypedTuple(named_colors->GetColor3ub("Tomato").GetData());
    colors->InsertNextTypedTuple(named_colors->GetColor3ub("Mint").GetData());

    line_poly_data->GetCellData()->SetScalars(colors);

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(line_poly_data);

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetLineWidth(4);

    renderer->AddActor(actor);
}
