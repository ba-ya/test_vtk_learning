#include "00headers.h"

#include <vtkArrowSource.h>
#include <vtkLabeledDataMapper.h>
#include <vtkMatrix4x4.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

void OrientedArrow3d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto arrow_source = vtkSmartPointer<vtkArrowSource>::New();

    // random startp and endp
    double start_point[3], end_point[3];
    auto rng = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
    rng->SetSeed(8775070);
    for (auto i = 0; i < 3; ++i) {
        rng->Next();
        start_point[i] = rng->GetRangeValue(-10, 10);
        rng->Next();
        end_point[i] = rng->GetRangeValue(-10, 10);
    }

    double normal_x[3], normal_y[3], normal_z[3];
    // x, a vector form start_point to end_point
    vtkMath::Subtract(end_point, start_point, normal_x);
    double length = vtkMath::Norm(normal_x);
    vtkMath::Normalize(normal_x);

    // z, a arbitrary(任意) vector cross x
    double arbitrary[3];
    for (int i = 0; i < 3; ++i) {
        rng->Next();
        arbitrary[i] = rng->GetRangeValue(-10, 10);
    }
    vtkMath::Cross(normal_x, arbitrary, normal_z);
    vtkMath::Normalize(normal_z);

    // y, z cross x
    vtkMath::Cross(normal_z, normal_x, normal_y);
    // vtkMath::Normalize(normal_y); // not need

    auto matrix = vtkSmartPointer<vtkMatrix4x4>::New();
    matrix->Identity();
    // s表示平移的坐标
    // | x1 y1 z1 s1 |
    // | x2 y2 z2 s2 |
    // | x3 y3 z3 s3 |
    // | 0  0  0  1  |
    for (int i = 0; i < 3; i++) {
        auto v = i == 0 ?  normal_x
                 : i == 1 ? normal_y : normal_z ;
        matrix->SetElement(0, i, length * v[0]);
        matrix->SetElement(1, i, length * v[1]);
        matrix->SetElement(2, i, length * v[2]);
    }
    matrix->SetElement(0, 3, start_point[0]);
    matrix->SetElement(1, 3, start_point[1]);
    matrix->SetElement(2, 3, start_point[2]);
    matrix->SetElement(3, 3, 1);

    auto transfrom = vtkSmartPointer<vtkTransform>::New();
    transfrom->SetMatrix(matrix);

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    auto actor = vtkSmartPointer<vtkActor>::New();

#if 0
    // no new data created
    mapper->SetInputConnection(arrow_source->GetOutputPort());
    actor->SetUserTransform(transfrom);
#else
    // create new polydata
    auto transform_pd = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transform_pd->SetTransform(transfrom);
    transform_pd->SetInputConnection(arrow_source->GetOutputPort());
    mapper->SetInputConnection(transform_pd->GetOutputPort());
#endif
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("Cyan").GetData());

    // sphere
    auto sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetRadius(.8);
    auto sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphere_mapper->SetInputConnection(sphere->GetOutputPort());

    auto sphere_start_actor = vtkSmartPointer<vtkActor>::New();
    sphere_start_actor->SetMapper(sphere_mapper);
    sphere_start_actor->SetPosition(start_point);
    sphere_start_actor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());

    auto sphere_end_actor = vtkSmartPointer<vtkActor>::New();
    sphere_end_actor->SetMapper(sphere_mapper);
    sphere_end_actor->SetPosition(end_point);
    sphere_end_actor->GetProperty()->SetColor(colors->GetColor3d("Magenta").GetData());

    renderer->AddActor(actor);
    renderer->AddActor(sphere_start_actor);
    renderer->AddActor(sphere_end_actor);


}
