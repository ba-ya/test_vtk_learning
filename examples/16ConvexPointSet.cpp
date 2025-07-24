#include "00headers.h"

#include <vtkConvexPointSet.h>
#include <vtkDataSetMapper.h>
#include <vtkGlyph3DMapper.h>
#include <vtkNamedColors.h>
#include <vtkPoints.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkUnstructuredGrid.h>

void ConvexPointSet3d::Draw(vtkRenderer *renderer)
{
    auto cps = vtkSmartPointer<vtkConvexPointSet>::New();
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 0, 0);
    points->InsertNextPoint(1, 1, 0);
    points->InsertNextPoint(0, 1, 0);
    points->InsertNextPoint(0, 0, 1);
    points->InsertNextPoint(1, 0, 1);
    points->InsertNextPoint(1, 1, 1);
    points->InsertNextPoint(0, 1, 1);// 立方体的8个点

    points->InsertNextPoint(0.5, 0, 0);
    points->InsertNextPoint(1, 0.5, 0);
    points->InsertNextPoint(0.5, 1, 0);
    points->InsertNextPoint(0, 0.5, 0);
    points->InsertNextPoint(0.5, 0.5, 0);

    for (int i = 0; i < 13; i++) {
        cps->GetPointIds()->InsertId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->Allocate(1, 1);
    ug->InsertNextCell(cps->GetCellType(), cps->GetPointIds());
    ug->SetPoints(points);

    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    /// ug
    auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(ug);
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
    // 显示edge
    actor->GetProperty()->SetLineWidth(3);
    actor->GetProperty()->EdgeVisibilityOn();
    renderer->AddActor(actor);

    /// points
    auto sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetPhiResolution(21);
    sphere->SetThetaResolution(21);
    sphere->SetRadius(.03);

    bool type = 0;
    if (type == 0) {
        auto polydata = vtkSmartPointer<vtkPolyData>::New();
        polydata->SetPoints(points);

        // Glyph 符号
        auto point_mapper = vtkSmartPointer<vtkGlyph3DMapper>::New();
        point_mapper->SetInputData(polydata);
        point_mapper->SetSourceConnection(sphere->GetOutputPort());

        auto point_actor = vtkSmartPointer<vtkActor>::New();
        point_actor->SetMapper(point_mapper);
        point_actor->GetProperty()->SetColor(colors->GetColor3d("Peacock").GetData());
        renderer->AddActor(point_actor);
    } else {
        for (int i = 0; i < 13; i++) {
            auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputConnection(sphere->GetOutputPort());

            auto actor = vtkSmartPointer<vtkActor>::New();
            actor->SetPosition(points->GetPoint(i));
            actor->SetMapper(mapper);
            actor->GetProperty()->SetColor(colors->GetColor3d("Peacock").GetData());
            renderer->AddActor(actor);
        }
    }
}
