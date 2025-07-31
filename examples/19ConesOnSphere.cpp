#include "00headers.h"

#include <vtkCameraOrientationWidget.h>
#include <vtkCellCenters.h>
#include <vtkConeSource.h>
#include <vtkGlyph3D.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>

void ConesOnSphere3d::Draw(vtkRenderer *renderer)
{
    auto sphere_source = vtkSmartPointer<vtkSphereSource>::New();
    sphere_source->SetRadius(.5);
    // 纬线数目,16 - 1
    sphere_source->SetPhiResolution(16);
    // 经线数目, 16
    sphere_source->SetThetaResolution(16);
    sphere_source->LatLongTessellationOn();

    auto cone_source = vtkSmartPointer<vtkConeSource>::New();
    cone_source->SetRadius(.08);
    cone_source->SetHeight(.03);
    cone_source->SetResolution(30);

    auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->ComputeCellNormalsOn();// 启用计算每个单元(面)的法向量
    normals->SetInputConnection(sphere_source->GetOutputPort());

    // 计算几何体中每个 Cell(面)中心的位置点。
    auto cell_centers = vtkSmartPointer<vtkCellCenters>::New();
    cell_centers->SetInputConnection(normals->GetOutputPort());

    // 在每个输入点位置放置一个"符号几何体"(glyph)，并可根据方向向量旋转它。
    auto glyph_filter = vtkSmartPointer<vtkGlyph3D>::New();
    glyph_filter->SetVectorModeToUseNormal(); // 使用法向量作为方向
    glyph_filter->OrientOn();                 // 开启方向调整
    glyph_filter->SetSourceConnection(cone_source->GetOutputPort()); // 设置要放置的符号形状(小 cone)
    glyph_filter->SetInputConnection(cell_centers->GetOutputPort()); // 设置 glyph 放置的位置(面中心点)。

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(glyph_filter->GetOutputPort());

    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->GetProperty()->SetColor(colors->GetColor3d("DeepSkyBlue").GetData());
    actor->GetProperty()->SetEdgeColor(colors->GetColor3d("DarkSlateBlue").GetData());
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
}
