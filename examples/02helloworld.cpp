#include "02helloworld.h"
#include "../common.h"

#include <vtkCylinderSource.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

void HelloWorld3d::Draw(vtkRenderer *renderer)
{
    // 绘制圆柱体
    auto cylinder = vtkSmartPointer<vtkCylinderSource>::New();
    // 圆柱体精度,越大越圆
    cylinder->SetResolution(8);

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(cylinder->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
#if 0
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
#else
    auto [r, g, b] = adj_rgb("#c24b36");
    actor->GetProperty()->SetColor(r, g, b);
#endif

    renderer->AddActor(actor);
}
