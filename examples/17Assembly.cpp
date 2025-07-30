#include "00headers.h"

#include <vtkAssembly.h>
#include <vtkCubeSource.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>

void Assembly3d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    // sphere
    auto sphere_source = vtkSmartPointer<vtkSphereSource>::New();
    sphere_source->Update();
    auto sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphere_mapper->SetInputConnection(sphere_source->GetOutputPort());
    auto sphere_actor = vtkSmartPointer<vtkActor>::New();
    sphere_actor->SetMapper(sphere_mapper);
    sphere_actor->GetProperty()->SetColor(colors->GetColor3d("Banana").GetData());

    // cube
    auto cube_source = vtkSmartPointer<vtkCubeSource>::New();
    cube_source->SetCenter(2,0,0);
    cube_source->Update();
    auto cube_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cube_mapper->SetInputConnection(cube_source->GetOutputPort());
    auto cube_actor = vtkSmartPointer<vtkActor>::New();
    cube_actor->SetMapper(cube_mapper);
    cube_actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
    cube_actor->GetProperty()->SetOpacity(.5);

    // assembly
    auto assembly = vtkSmartPointer<vtkAssembly>::New();
    assembly->AddPart(sphere_actor);
    assembly->AddPart(cube_actor);

    renderer->AddActor(assembly);
}
