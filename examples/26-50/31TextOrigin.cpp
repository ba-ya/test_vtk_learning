#include "00headers.h"

#include <vtkAxes.h>
#include <vtkFollower.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkVectorText.h>

void TextOrigin3d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto axes = vtkSmartPointer<vtkAxes>::New();
    axes->SetOrigin(0,0,0);
    auto axes_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    axes_mapper->SetInputConnection(axes->GetOutputPort());
    auto axes_actor = vtkSmartPointer<vtkActor>::New();
    axes_actor->SetMapper(axes_mapper);

    auto text = vtkSmartPointer<vtkVectorText>::New();
    text->SetText("Origin");
    auto text_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    text_mapper->SetInputConnection(text->GetOutputPort());
    auto text_actor = vtkSmartPointer<vtkFollower>::New();
    text_actor->SetMapper(text_mapper);
    text_actor->SetScale(.2, .2, .2);
    // setCamera to make text follow
    text_actor->SetCamera(renderer->GetActiveCamera());

    renderer->AddActor(axes_actor);
    renderer->AddActor(text_actor);
}
