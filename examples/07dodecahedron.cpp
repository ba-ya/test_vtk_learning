#include "00headers.h"

#include <vtkNamedColors.h>
#include <vtkPoints.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyhedron.h>
#include <vtkProperty.h>

void Dodecahedron3d::Draw(vtkRenderer *renderer)
{
    // vtkPolyhedron不同于polydata,是专门用于封闭三维体的一个类，它允许：
    // 使用 一个顶点列表
    // 使用 一个面列表
    // 自动形成一个封闭体，适用于体渲染或布尔操作等高级功能
    auto dodecahedron = vtkSmartPointer<vtkPolyhedron>::New();

    // 正12面体, 20个顶点
    for (int i = 0; i < 20; ++i) {
        dodecahedron->GetPointIds()->InsertNextId(i);
    }

    dodecahedron->GetPoints()->InsertNextPoint(1.21412, 0, 1.58931);
    dodecahedron->GetPoints()->InsertNextPoint(0.375185, 1.1547, 1.58931);
    dodecahedron->GetPoints()->InsertNextPoint(-0.982247, 0.713644, 1.58931);
    dodecahedron->GetPoints()->InsertNextPoint(-0.982247, -0.713644, 1.58931);
    dodecahedron->GetPoints()->InsertNextPoint(0.375185, -1.1547, 1.58931);
    dodecahedron->GetPoints()->InsertNextPoint(1.96449, 0, 0.375185);
    dodecahedron->GetPoints()->InsertNextPoint(0.607062, 1.86835, 0.375185);
    dodecahedron->GetPoints()->InsertNextPoint(-1.58931, 1.1547, 0.375185);
    dodecahedron->GetPoints()->InsertNextPoint(-1.58931, -1.1547, 0.375185);
    dodecahedron->GetPoints()->InsertNextPoint(0.607062, -1.86835, 0.375185);
    dodecahedron->GetPoints()->InsertNextPoint(1.58931, 1.1547, -0.375185);
    dodecahedron->GetPoints()->InsertNextPoint(-0.607062, 1.86835, -0.375185);
    dodecahedron->GetPoints()->InsertNextPoint(-1.96449, 0, -0.375185);
    dodecahedron->GetPoints()->InsertNextPoint(-0.607062, -1.86835, -0.375185);
    dodecahedron->GetPoints()->InsertNextPoint(1.58931, -1.1547, -0.375185);
    dodecahedron->GetPoints()->InsertNextPoint(0.982247, 0.713644, -1.58931);
    dodecahedron->GetPoints()->InsertNextPoint(-0.375185, 1.1547, -1.58931);
    dodecahedron->GetPoints()->InsertNextPoint(-1.21412, 0, -1.58931);
    dodecahedron->GetPoints()->InsertNextPoint(-0.375185, -1.1547, -1.58931);
    dodecahedron->GetPoints()->InsertNextPoint(0.982247, -0.713644, -1.58931);

    // 1 + (5 + 1) * 12;
    vtkIdType faces[1 + (5 + 1) * 12] = {
                                         12,                    // number of faces
                                         5,  0,  1,  2,  3,  4, // number of ids on face, ids
                                         5,  0,  5,  10, 6,  1,
                                         5,  1,  6,  11, 7,  2,
                                         5,  2,  7,  12, 8,  3,
                                         5,  3,  8,  13, 9,  4,
                                         5,  4,  9,  14, 5,  0,
                                         5,  15, 10, 5,  14, 19,
                                         5,  16, 11, 6,  10, 15,
                                         5,  17, 12, 7,  11, 16,
                                         5,  18, 13, 8,  12, 17,
                                         5,  19, 14, 9,  13, 18,
                                         5,  19, 18, 17, 16, 15};

    dodecahedron->SetFaces(faces);
    dodecahedron->Initialize();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(dodecahedron->GetPolyData());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    actor->GetProperty()->SetColor(colors->GetColor3d("PapayaWhip").GetData());

    renderer->AddActor(actor);
}
