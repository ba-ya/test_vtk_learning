#include "00headers.h"

#include <vtkEarthSource.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>

void EarthSource3d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto earth_source = vtkSmartPointer<vtkEarthSource>::New();
    earth_source->OutlineOn();
    earth_source->Update();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(earth_source->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

    // sphere
    auto sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetThetaResolution(100);
    sphere->SetPhiResolution(100);
    sphere->SetRadius(earth_source->GetRadius());

    auto sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphere_mapper->SetInputConnection(sphere->GetOutputPort());

    auto sphere_actor = vtkSmartPointer<vtkActor>::New();
    sphere_actor->SetMapper(sphere_mapper);
    sphere_actor->GetProperty()->SetColor(colors->GetColor3d("PeachPuff").GetData());

    renderer->AddActor(actor);
    renderer->AddActor(sphere_actor);

}
