#include "00headers.h"

#include <vtkConeSource.h>
#include <vtkCubeAxesActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

void DetermineActorType3d::Draw(vtkRenderer *renderer)
{
    auto cube_axes_actor = vtkSmartPointer<vtkCubeAxesActor>::New();
    cube_axes_actor->SetCamera(renderer->GetActiveCamera());
    cube_axes_actor->SetFlyModeToOuterEdges();

    auto cone = vtkSmartPointer<vtkConeSource>::New();
    auto cone_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cone_mapper->SetInputConnection(cone->GetOutputPort());
    auto cone_actor = vtkSmartPointer<vtkActor>::New();
    cone_actor->SetMapper(cone_mapper);

    renderer->AddActor(cube_axes_actor);
    renderer->AddActor(cone_actor);

}
