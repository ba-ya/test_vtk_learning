#include "00headers.h"

#include <vtkActor2D.h>
#include <vtkArrowSource.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkDiskSource.h>
#include <vtkLineSource.h>
#include <vtkNamedColors.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRegularPolygonSource.h>
#include <vtkSphereSource.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>

void GeometricObjectsDemo3d::Draw(std::vector<vtkSmartPointer<vtkRenderer> > renders)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    std::vector<vtkSmartPointer<vtkPolyDataAlgorithm>> geometric_object_sources;
    geometric_object_sources.push_back(vtkSmartPointer<vtkArrowSource>::New()); // 箭头
    geometric_object_sources.push_back(vtkSmartPointer<vtkConeSource>::New()); // 锥
    geometric_object_sources.push_back(vtkSmartPointer<vtkCubeSource>::New()); // 立方体
    geometric_object_sources.push_back(vtkSmartPointer<vtkCylinderSource>::New()); // 圆柱体
    geometric_object_sources.push_back(vtkSmartPointer<vtkDiskSource>::New()); // 磁盘
    geometric_object_sources.push_back(vtkSmartPointer<vtkLineSource>::New()); //线
    geometric_object_sources.push_back(vtkSmartPointer<vtkRegularPolygonSource>::New()); // 正多边形
    geometric_object_sources.push_back(vtkSmartPointer<vtkSphereSource>::New()); // 球体

    // 准备mapper, actor, 和 文字说明
    std::vector<vtkSmartPointer<vtkPolyDataMapper>> mappers;
    std::vector<vtkSmartPointer<vtkActor>> actors;
    std::vector<vtkSmartPointer<vtkTextMapper>> text_mappers;
    std::vector<vtkSmartPointer<vtkActor2D>> text_actors;

    // 文字说明属性
    auto text_property = vtkSmartPointer<vtkTextProperty>::New();
    text_property->SetFontSize(16);
    text_property->SetJustificationToCentered();
    text_property->SetColor(colors->GetColor3d("LightGoldenrodYellow").GetData());

    // 准备 mapper 和 actor
    for (int i = 0; i < geometric_object_sources.size(); ++i) {
        // vtkSphereSource 等数据源类（vtkPolyDataAlgorithm 的子类）
        // 内部维护着输出数据结构（比如点、面、拓扑等），
        // 但这些数据默认是惰性计算的
        // 需要通过Update()强制生成几何数据
        // 不过如果是mapper->SetInputConnection(xxSource->GetOutputPort())这样使用,可以省略,Render的时候会自动Update()
        geometric_object_sources[i]->Update();

        mappers.push_back(vtkSmartPointer<vtkPolyDataMapper>::New());
        mappers[i]->SetInputConnection(geometric_object_sources[i]->GetOutputPort());

        actors.push_back(vtkSmartPointer<vtkActor>::New());
        actors[i]->SetMapper(mappers[i]);
        actors[i]->GetProperty()->SetColor(colors->GetColor3d("PeachPuff").GetData());

        text_mappers.push_back(vtkSmartPointer<vtkTextMapper>::New());
        text_mappers[i]->SetInput(geometric_object_sources[i]->GetClassName());
        text_mappers[i]->SetTextProperty(text_property);

        text_actors.push_back(vtkSmartPointer<vtkActor2D>::New());
        text_actors[i]->SetMapper(text_mappers[i]);
        text_actors[i]->SetPosition(120, 16);
    }

    int grid_cols = 3, grid_rows = 3;
    for (int row = 0; row < grid_rows; ++row) {
        for (int col = 0; col < grid_cols; ++col) {
            auto index = row * grid_cols + col;
            auto viewport = get_viewport(col, row, grid_cols, grid_rows);
            if (index < geometric_object_sources.size()) {
                renders[index]->SetViewport(viewport.data() );
                renders[index]->AddActor(actors[index]);
                renders[index]->AddActor2D(text_actors[index]);
            }
        }
    }
}
