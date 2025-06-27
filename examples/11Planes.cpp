#include "00headers.h"

#include <vtkCamera.h>
#include <vtkHull.h>
#include <vtkNamedColors.h>
#include <vtkPlanes.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkTextProperty.h>
#include <vtkActor2D.h>
#include <vtkTextMapper.h>

void Planes3d::Draw(std::vector<vtkSmartPointer<vtkRenderer>> renders)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    QStringList titles = {
        "Using frustum planes",
        "Using bounds",
    };
    std::vector<vtkSmartPointer<vtkPlanes>> planes;
    for (int i = 0; i < titles.size(); ++i) {
        planes.push_back(vtkSmartPointer<vtkPlanes>::New());
    }

    /// frustum, 相机视锥体,金字塔截去顶部
    auto camera = vtkSmartPointer<vtkCamera>::New();
    // 获取相机的六个裁剪面
    // 每个面由 4 个 double 表示一个平面方程(Ax + By + Cz + D = 0)，共 6 面 × 4 = 24 个值。
    double plane_array[24];
    camera->GetFrustumPlanes(1, plane_array);
    planes[0]->SetFrustumPlanes(plane_array);

    /// bounds, 包围盒
    auto sphere_source = vtkSmartPointer<vtkSphereSource>::New();
    sphere_source->Update();
    double bounds[6];
    sphere_source->GetOutput()->GetBounds(bounds);
    planes[1]->SetBounds(bounds);

    auto text_property = vtkSmartPointer<vtkTextProperty>::New();
    text_property->SetFontSize(16);
    text_property->SetJustificationToLeft();

    std::vector<vtkSmartPointer<vtkActor>> actors;
    std::vector<vtkSmartPointer<vtkActor2D>> text_actors;
    for (int i = 0; i < titles.size(); ++i) {
        //  从一组平面，生成几何体（convex hull）
        auto hull = vtkSmartPointer<vtkHull>::New();
        hull->SetPlanes(planes[i]);
        auto polydata =vtkSmartPointer<vtkPolyData>::New();
        hull->GenerateHull(polydata, -200, 200, -200, 200, -200, 200);

        auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(polydata);

        auto actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->SetColor(colors->GetColor3d("moccasin").GetData());
        actors.push_back(actor);

        auto text_mapper = vtkSmartPointer<vtkTextMapper>::New();
        text_mapper->SetInput(titles.at(i).toUtf8().constData());
        text_mapper->SetTextProperty(text_property);

        auto text_actor = vtkSmartPointer<vtkActor2D>::New();
        text_actor->SetMapper(text_mapper);
        // 像素点位置,距离左边120px, 距离底部16px
        text_actor->SetPosition(120, 16);
        text_actors.push_back(text_actor);

    }
    Helper::layout_renders_in_grid(renders, actors, text_actors, 2, 1);
}
