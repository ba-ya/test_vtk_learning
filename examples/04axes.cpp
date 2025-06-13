#include "00headers.h"

#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkTransform.h>

void Axes3d::Draw(vtkRenderer *renderer)
{
#if 0
    // sphere 球体
    auto sphere_source = vtkSmartPointer<vtkSphereSource>::New();
    sphere_source->SetCenter(0.0, 0.0, 0.0);
    sphere_source->SetRadius(0.5);

    auto sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphere_mapper->SetInputConnection(sphere_source->GetOutputPort());

    auto sphere_actor = vtkSmartPointer<vtkActor>::New();
    sphere_actor->SetMapper(sphere_mapper);
    // auto [r, g, b] = adj_rgb("#c24b36");
    // sphere_actor->GetProperty()->SetColor(r, g, b);

    renderer->AddActor(sphere_actor);
#endif
    // 轴
    auto transform = vtkSmartPointer<vtkTransform>::New();
    transform->Translate(0.0, 0.0, 0.0);// 平移

    auto axes = vtkSmartPointer<vtkAxesActor>::New();
    axes->SetUserTransform(transform);

    renderer->AddActor(axes);
}
