#include "00headers.h"

#include <vtkCamera.h>
#include <vtkFrustumSource.h>
#include <vtkNamedColors.h>
#include <vtkPlanes.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkShrinkPolyData.h>

void Frustum3d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto camera = vtkSmartPointer<vtkCamera>::New();
    camera->SetClippingRange(.1, .4);
    double plane_array[24];
    camera->GetFrustumPlanes(1, plane_array);

    auto planes = vtkSmartPointer<vtkPlanes>::New();
    planes->SetFrustumPlanes(plane_array);

    auto frustum_source = vtkSmartPointer<vtkFrustumSource>::New();
    frustum_source->ShowLinesOff();
    frustum_source->SetPlanes(planes);

    auto shrink = vtkSmartPointer<vtkShrinkPolyData>::New();
    shrink->SetInputConnection(frustum_source->GetOutputPort());
    shrink->SetShrinkFactor(.9);

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(shrink->GetOutputPort());

    auto back = vtkSmartPointer<vtkProperty>::New();
    back->SetColor(colors->GetColor3d("Tomato").GetData());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("Banana").GetData());
    actor->SetBackfaceProperty(back);

    renderer->AddActor(actor);
}
