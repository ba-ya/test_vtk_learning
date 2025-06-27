#include "00headers.h"

#include <vtkCubeSource.h>
#include <vtkDataSetMapper.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkShrinkFilter.h>

void ShrinkCube3d::Draw(vtkRenderer *renderer)
{
    auto cube_source = vtkSmartPointer<vtkCubeSource>::New();

    auto shrink = vtkSmartPointer<vtkShrinkFilter>::New();
    shrink->SetInputConnection(cube_source->GetOutputPort());;
    shrink->SetShrinkFactor(.9);

    auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputConnection(shrink->GetOutputPort());

    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto back = vtkSmartPointer<vtkProperty>::New();
    back->SetColor(colors->GetColor3d("Tomato").GetData());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    // 每条边添加黑色边框
    actor->GetProperty()->EdgeVisibilityOn();
    actor->GetProperty()->SetColor(colors->GetColor3d("Banana").GetData());
    actor->SetBackfaceProperty(back);

    renderer->AddActor(actor);

}
