#include "00headers.h"

#include <vtkCylinderSource.h>
#include <vtkMatrix4x4.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRandomSequence.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

void OrientedCylinder3d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto cylinder_source = vtkSmartPointer<vtkCylinderSource>::New();
    cylinder_source->SetResolution(15);

    double start_p[3], end_p[3];
    auto rng = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
    rng->SetSeed(8775070);
    for (int i = 0; i < 3; ++i) {
        start_p[i] = rng->GetNextRangeValue(-10, 10);
        end_p[i] = rng->GetNextRangeValue(-10, 10);
    }

    double normal_x[3], normal_y[3], normal_z[3];
    // y, a vector from strat to end
    vtkMath::Subtract(end_p, start_p, normal_y);
    auto length = vtkMath::Norm(normal_y);
    vtkMath::Normalize(normal_y);

    // z, a arbitrary vector cross y
    double arbitrary[3];
    for(int i = 0; i < 3; ++i) {
        arbitrary[i] = rng->GetNextRangeValue(-10, 10);
    }
    vtkMath::Cross(arbitrary, normal_y, normal_z);
    vtkMath::Normalize(normal_z);

    // x, y cross z
    vtkMath::Cross(normal_y, normal_z, normal_x);

    // matrix
    auto matrix = vtkSmartPointer<vtkMatrix4x4>::New();
    matrix->Identity();
    // s表示平移的坐标
    // | x1 y1 z1 s1 |
    // | x2 y2 z2 s2 |
    // | x3 y3 z3 s3 |
    // | 0  0  0  1  |
    QList<double> lis_ratio = {1, length, 1};
    for (int i = 0; i < 3; ++i) {
        auto vector = i == 0 ? normal_x
                      : i == 1 ? normal_y : normal_z;
        auto ratio = lis_ratio[i];
        matrix->SetElement(0, i, ratio * vector[0]);
        matrix->SetElement(1, i, ratio * vector[1]);
        matrix->SetElement(2, i, ratio * vector[2]);
    }
    matrix->SetElement(0, 3, (start_p[0] + end_p[0]) / 2);
    matrix->SetElement(1, 3, (start_p[1] + end_p[1]) / 2);
    matrix->SetElement(2, 3, (start_p[2] + end_p[2]) / 2);
    matrix->SetElement(3, 3, 1);

    auto transform = vtkSmartPointer<vtkTransform>::New();
    transform->SetMatrix(matrix);

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    auto actor = vtkSmartPointer<vtkActor>::New();
#if 1
    mapper->SetInputConnection(cylinder_source->GetOutputPort());
    actor->SetUserTransform(transform);
#else
    auto transform_pd = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transform_pd->SetInputConnection(cylinder_source->GetOutputPort());
    transform_pd->SetTransform(transform);
    mapper->SetInputConnection(transform_pd->GetOutputPort());
#endif
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("Cyan").GetData());

    // sphere
    auto sphere_source = vtkSmartPointer<vtkSphereSource>::New();
    sphere_source->SetRadius(.8);
    auto sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphere_mapper->SetInputConnection(sphere_source->GetOutputPort());

    auto sphere_start_actor = vtkSmartPointer<vtkActor>::New();
    sphere_start_actor->SetMapper(sphere_mapper);
    sphere_start_actor->SetPosition(start_p);
    sphere_start_actor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());

    auto sphere_end_actor = vtkSmartPointer<vtkActor>::New();
    sphere_end_actor->SetMapper(sphere_mapper);
    sphere_end_actor->SetPosition(end_p);
    sphere_end_actor->GetProperty()->SetColor(colors->GetColor3d("Magenta").GetData());

    renderer->AddActor(actor);
    renderer->AddActor(sphere_start_actor);
    renderer->AddActor(sphere_end_actor);
}
