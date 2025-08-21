#include "00headers.h"

#include <vtkArrowSource.h>
#include <vtkBrownianPoints.h>
#include <vtkGlyph3D.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>

void BrownianPoints3d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    // sphere
    auto sphere_source = vtkSmartPointer<vtkSphereSource>::New();

    auto sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphere_mapper->SetInputConnection(sphere_source->GetOutputPort());

    auto sphere_actor = vtkSmartPointer<vtkActor>::New();
    sphere_actor->SetMapper(sphere_mapper);
    sphere_actor->GetProperty()->EdgeVisibilityOn();

    // arrow as glyph
    // vtkBrownianPoints 在 VTK 里是一个 滤波器 (filter)，
    // 它的主要作用是 为每个点生成一个“随机运动（布朗运动）”的向量场。
    vtkMath::RandomSeed(5070);
    auto brownian_points = vtkSmartPointer<vtkBrownianPoints>::New();
    brownian_points->SetInputConnection(sphere_source->GetOutputPort());

    auto arrow_source = vtkSmartPointer<vtkArrowSource>::New();

    auto glyph = vtkSmartPointer<vtkGlyph3D>::New();
    // glyph->SetVectorModeToUseNormal();
    glyph->SetSourceConnection(arrow_source->GetOutputPort());
    glyph->SetInputConnection(brownian_points->GetOutputPort());
    glyph->SetScaleFactor(.3);

    auto glyph_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    glyph_mapper->SetInputConnection(glyph->GetOutputPort());

    auto glyph_actor = vtkSmartPointer<vtkActor>::New();
    glyph_actor->SetMapper(glyph_mapper);
    glyph_actor->GetProperty()->SetColor(colors->GetColor3d("Banana").GetData());

    renderer->AddActor(sphere_actor);
    renderer->AddActor(glyph_actor);
}

