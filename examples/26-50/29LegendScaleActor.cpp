#include "00headers.h"

#include <vtkLegendScaleActor.h>
#include <vtkNamedColors.h>
#include <vtkParametricEnneper.h>
#include <vtkParametricFunctionSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

void LegendScaleActor2d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto src = vtkSmartPointer<vtkParametricEnneper>::New();
    auto fn_src = vtkSmartPointer<vtkParametricFunctionSource>::New();
    fn_src->SetParametricFunction(src);

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(fn_src->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("SandyBrown").GetData());

    auto legend_scale_actor = vtkSmartPointer<vtkLegendScaleActor>::New();
    legend_scale_actor->LegendVisibilityOn();
    legend_scale_actor->SetLabelModeToDistance();

    renderer->AddActor(actor);
    renderer->AddActor(legend_scale_actor);
}
