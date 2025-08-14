#include "00headers.h"

#include <vtkDataSetMapper.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkShrinkFilter.h>
#include <vtkTessellatedBoxSource.h>

void TessellatedBoxSource3d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    double bounds[6] = {-10, 10,
                         10, 20,
                        -5, 5};

    auto box_source = vtkSmartPointer<vtkTessellatedBoxSource>::New();
    box_source->SetLevel(3);
    box_source->QuadsOff();
    box_source->SetBounds(bounds);
    box_source->SetOutputPointsPrecision(vtkAlgorithm::SINGLE_PRECISION);

    auto shrink = vtkSmartPointer<vtkShrinkFilter>::New();
    shrink->SetInputConnection(box_source->GetOutputPort());
    shrink->SetShrinkFactor(.8);

    auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputConnection(shrink->GetOutputPort());

    auto back = vtkSmartPointer<vtkProperty>::New();
    back->SetColor(colors->GetColor3d("Tomato").GetData());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->SetBackfaceProperty(back);
    actor->GetProperty()->EdgeVisibilityOn();
    actor->GetProperty()->SetColor(colors->GetColor3d("Banana").GetData());

    renderer->AddActor(actor);
}
