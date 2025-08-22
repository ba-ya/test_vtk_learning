#include "00headers.h"

#include <vtkDelaunay2D.h>
#include <vtkLabeledDataMapper.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolygon.h>
#include <vtkProperty.h>

void ConstrainedDelaunay2D::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto points = vtkSmartPointer<vtkPoints>::New();
    int grid_size = 10;
    int seed = 0;
    auto random_sequence = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
    random_sequence->Initialize(seed);

    for (int x = 0; x < grid_size; ++x) {
        for (int y = 0; y < grid_size; ++y) {
            auto d1 = random_sequence->GetNextValue() / 2.0 - 0.25;
            auto d2 = random_sequence->GetNextValue() / 2.0 - 0.25;
            d1 = d2 = 0;
            points->InsertNextPoint(x + d1, y + d2, 0);
        }
    }
    auto polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);

    auto cell_array = vtkSmartPointer<vtkCellArray>::New();

    std::vector<std::vector<int>> polygon_ids = {
        {22, 23, 24, 34, 44, 43, 42, 32,},
        {55, 56, 57, 58, 68, 67, 66, 76, 86, 85, 75, 65,},
        {51, 52, 62, 63, 73, 72, 82, 81, 71, 61},
        };
    std::vector<vtkSmartPointer<vtkPolygon>> polygons;
    for (int i = 0; i < polygon_ids.size(); ++i) {
        auto polygon = vtkSmartPointer<vtkPolygon>::New();
        auto ids = polygon_ids[i];
        for (int j = 0; j < ids.size(); ++j) {
            polygon->GetPointIds()->InsertNextId(ids[j]);
        }
        polygons.push_back(polygon);
    }

    for (auto &polygon : polygons) {
        cell_array->InsertNextCell(polygon);
    }

    auto boundary = vtkSmartPointer<vtkPolyData>::New();
    boundary->SetPoints(polydata->GetPoints());
    boundary->SetPolys(cell_array);

    auto delaunay = vtkSmartPointer<vtkDelaunay2D>::New();
    delaunay->SetInputData(polydata);
    delaunay->SetSourceData(boundary);

    auto mesh_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mesh_mapper->SetInputConnection(delaunay->GetOutputPort());

    auto mesh_actor = vtkSmartPointer<vtkActor>::New();
    mesh_actor->SetMapper(mesh_mapper);
    mesh_actor->GetProperty()->EdgeVisibilityOn();
    mesh_actor->GetProperty()->SetEdgeColor(colors->GetColor3d("Peacock").GetData());

    auto boundary_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    boundary_mapper->SetInputData(boundary);

// useless
#if 0
    auto boundary_actor = vtkSmartPointer<vtkActor>::New();
    boundary_actor->SetMapper(boundary_mapper);
    boundary_actor->GetProperty()->SetColor(colors->GetColor3d("Raspberry").GetData());
    boundary_actor->GetProperty()->EdgeVisibilityOn();
    boundary_actor->GetProperty()->SetEdgeColor(colors->GetColor3d("Red").GetData());
    boundary_actor->GetProperty()->SetRepresentationToWireframe();
#endif
    auto label_mapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
    label_mapper->SetInputData(polydata);

    auto label_actor = vtkSmartPointer<vtkActor2D>::New();
    label_actor->SetMapper(label_mapper);

    renderer->AddActor(mesh_actor);
    // renderer->AddActor(boundary_actor);
    renderer->AddActor2D(label_actor);
}
