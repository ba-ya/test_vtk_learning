#include "00headers.h"

#include <vtkNamedColors.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

void Plane2d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto plane_source = vtkSmartPointer<vtkPlaneSource>::New();
    plane_source->SetCenter(1, 0, 0);
    plane_source->SetNormal(1, 0, 1);

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if 1
    mapper->SetInputConnection(plane_source->GetOutputPort());
#else
    // it needs to Update() when setting polydata
    plane_source->Update();
    mapper->SetInputData(plane_source->GetOutput());
#endif

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());

    renderer->AddActor(actor);
}
