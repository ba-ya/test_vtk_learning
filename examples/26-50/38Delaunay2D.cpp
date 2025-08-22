#include "00headers.h"

#include <vtkDelaunay2D.h>
#include <vtkLabeledDataMapper.h>
#include <vtkNamedColors.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMApper.h>
#include <vtkProperty.h>

void Delaunay2D::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto points = vtkSmartPointer<vtkPoints>::New();
    int grid_size = 10;
    for (int x = 0; x < grid_size; ++x) {
        for (int y = 0; y < grid_size; ++y) {
            points->InsertNextPoint(x, y, (x + y) / (y + 1));
        }
    }
    auto polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);

    auto delaunay = vtkSmartPointer<vtkDelaunay2D>::New();
    delaunay->SetInputData(polydata);

    auto mesh_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mesh_mapper->SetInputConnection(delaunay->GetOutputPort());

    auto mesh_actor = vtkSmartPointer<vtkActor>::New();
    mesh_actor->SetMapper(mesh_mapper);
    mesh_actor->GetProperty()->EdgeVisibilityOn();
    mesh_actor->GetProperty()->SetLineWidth(2);
    mesh_actor->GetProperty()->SetEdgeColor(colors->GetColor3d("Raspberry").GetData());

    auto label_mapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
    label_mapper->SetInputData(polydata);

    auto label_actor = vtkSmartPointer<vtkActor2D>::New();
    label_actor->SetMapper(label_mapper);

    renderer->AddActor(mesh_actor);
    renderer->AddActor2D(label_actor);
}
