#include "00headers.h"

#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRegularPolygonSource.h>
#include <vtkShrinkPolyData.h>

void RegularPolygonSource2d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto polygon_source = vtkSmartPointer<vtkRegularPolygonSource>::New();
    polygon_source->SetNumberOfSides(6);
    polygon_source->SetRadius(5);
    polygon_source->SetCenter(0,0,0);

    auto shrink = vtkSmartPointer<vtkShrinkPolyData>::New();
    shrink->SetInputConnection(polygon_source->GetOutputPort());
    shrink->SetShrinkFactor(.9);

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(shrink->GetOutputPort());

    auto back = vtkSmartPointer<vtkProperty>::New();
    back->SetColor(colors->GetColor3d("Tomato").GetData());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->EdgeVisibilityOn();
    actor->GetProperty()->SetLineWidth(5);
    actor->GetProperty()->SetColor(colors->GetColor3d("Banana").GetData());
    actor->SetBackfaceProperty(back);

    renderer->AddActor(actor);
}
