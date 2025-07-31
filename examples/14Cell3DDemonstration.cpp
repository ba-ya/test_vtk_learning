#include "00headers.h"

#include <vtkDataSetMapper.h>
#include <vtkNamedColors.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkProperty.h>
#include <vtkHexagonalPrism.h>
#include <vtkHexahedron.h>
#include <vtkPentagonalPrism.h>
#include <vtkWedge.h>
#include <vtkVoxel.h>
#include <vtkTetra.h>
#include <vtkPyramid.h>

namespace {
// 非结构化网格
vtkSmartPointer<vtkUnstructuredGrid> MakeHexagonalPrism();
vtkSmartPointer<vtkUnstructuredGrid> MakeHexahedron();
vtkSmartPointer<vtkUnstructuredGrid> MakePentagonalPrism();

vtkSmartPointer<vtkUnstructuredGrid> MakePolyhedron();
vtkSmartPointer<vtkUnstructuredGrid> MakePyramid();
vtkSmartPointer<vtkUnstructuredGrid> MakeTetrahedron();

vtkSmartPointer<vtkUnstructuredGrid> MakeVoxel();
vtkSmartPointer<vtkUnstructuredGrid> MakeWedge();
}

void Cell3DDemonstration3d::Draw(std::vector<vtkSmartPointer<vtkRenderer>> renders)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    std::vector<vtkSmartPointer<vtkUnstructuredGrid>> ugrids;
    std::vector<vtkSmartPointer<vtkActor>> actors;
    std::vector<vtkSmartPointer<vtkActor2D>> text_actors;
    QStringList titles = {
        "HexagonalPrismHexagonalPrism",
        "HexahedronHexahedron",
        "PentagonalPrismPentagonalPrism",
        "PolyhedronPolyhedron",
        "PyramidPyramid",
        "TetrahedronTetrahedron",
        "VoxelVoxel",
        "WedgeWedge",
    };
    ugrids.push_back(MakeHexagonalPrism());
    ugrids.push_back(MakeHexahedron());
    ugrids.push_back(MakePentagonalPrism());
    ugrids.push_back(MakePolyhedron());
    ugrids.push_back(MakePyramid());
    ugrids.push_back(MakeTetrahedron());
    ugrids.push_back(MakeVoxel());
    ugrids.push_back(MakeWedge());

    auto text_property = vtkSmartPointer<vtkTextProperty>::New();
    text_property->SetFontSize(16);
    text_property->SetJustificationToCentered();
    text_property->SetColor(colors->GetColor3d("LightGoldenrodYellow").GetData());

    for (int i = 0; i < ugrids.size(); ++i) {
        auto text_mapper = vtkSmartPointer<vtkTextMapper>::New();
        text_mapper->SetInput(titles[i].toUtf8().constData());
        text_mapper->SetTextProperty(text_property);

        text_actors.push_back(vtkSmartPointer<vtkActor2D>::New());
        text_actors[i]->SetMapper(text_mapper);

        auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        mapper->SetInputData(ugrids[i]);

        actors.push_back(vtkSmartPointer<vtkActor>::New());
        actors[i]->SetMapper(mapper);
        actors[i]->GetProperty()->SetColor(colors->GetColor3d("Peachpuff").GetData());
        // 显示edge
        actors[i]->GetProperty()->SetLineWidth(3);
        actors[i]->GetProperty()->EdgeVisibilityOn();

        renders[i]->AddActor(actors[i]);
        renders[i]->AddActor2D(text_actors[i]);
    }
}

namespace {
// 上下两个六边形
vtkSmartPointer<vtkUnstructuredGrid> MakeHexagonalPrism() {
    int cnt_vertices = 12;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0.0, 0.0, 1.0);
    points->InsertNextPoint(1.0, 0.0, 1.0);
    points->InsertNextPoint(1.5, 0.5, 1.0);
    points->InsertNextPoint(1.0, 1.0, 1.0);
    points->InsertNextPoint(0.0, 1.0, 1.0);
    points->InsertNextPoint(-0.5, 0.5, 1.0);

    points->InsertNextPoint(0.0, 0.0, 0.0);
    points->InsertNextPoint(1.0, 0.0, 0.0);
    points->InsertNextPoint(1.5, 0.5, 0.0);
    points->InsertNextPoint(1.0, 1.0, 0.0);
    points->InsertNextPoint(0.0, 1.0, 0.0);
    points->InsertNextPoint(-0.5, 0.5, 0.0);

    // 六边形棱柱
    auto hexagonal_prism = vtkSmartPointer<vtkHexagonalPrism>::New();
    for (int i = 0; i < cnt_vertices; ++i) {
        hexagonal_prism->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->InsertNextCell(hexagonal_prism->GetCellType(), hexagonal_prism->GetPointIds());
    ug->SetPoints(points);

    qDebug() << __FUNCTION__ << hexagonal_prism->GetCellType();
    return ug;
}

// 两个四边形
vtkSmartPointer<vtkUnstructuredGrid> MakeHexahedron() {
    int cnt_vertices = 8;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0.0, 0.0, 0.0);
    points->InsertNextPoint(1.0, 0.0, 0.0);
    points->InsertNextPoint(1.0, 1.0, 0.0);
    points->InsertNextPoint(0.0, 1.0, 0.0);

    points->InsertNextPoint(0.0, 0.0, 1.0);
    points->InsertNextPoint(1.0, 0.0, 1.0);
    points->InsertNextPoint(1.0, 1.0, 1.0);
    points->InsertNextPoint(0.0, 1.0, 1.0);

    // 六面体
    auto hex = vtkSmartPointer<vtkHexahedron>::New();
    for (int i = 0; i < cnt_vertices; ++i) {
        hex->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->InsertNextCell(hex->GetCellType(), hex->GetPointIds());
    ug->SetPoints(points);

    qDebug() << __FUNCTION__ << hex->GetCellType();
    return ug;
}

// 两个五边形
vtkSmartPointer<vtkUnstructuredGrid> MakePentagonalPrism() {
    int cnt_vertices = 10;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(11, 10, 10);
    points->InsertNextPoint(13, 10, 10);
    points->InsertNextPoint(14, 12, 10);
    points->InsertNextPoint(12, 14, 10);
    points->InsertNextPoint(10, 12, 10);

    points->InsertNextPoint(11, 10, 14);
    points->InsertNextPoint(13, 10, 14);
    points->InsertNextPoint(14, 12, 14);
    points->InsertNextPoint(12, 14, 14);
    points->InsertNextPoint(10, 12, 14);

    // 五边形棱柱
    auto pentagonal_prism = vtkSmartPointer<vtkPentagonalPrism>::New();
    for (int i = 0; i < cnt_vertices; ++i) {
        pentagonal_prism->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->InsertNextCell(pentagonal_prism->GetCellType(), pentagonal_prism->GetPointIds());
    ug->SetPoints(points);
    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakePolyhedron() {
    int cnt_vertices = 20;
    int cnt_faces = 12;
    int cnt_face_vertices = 5;

    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(1.21412, 0, 1.58931);
    points->InsertNextPoint(0.375185, 1.1547, 1.58931);
    points->InsertNextPoint(-0.982247, 0.713644, 1.58931);
    points->InsertNextPoint(-0.982247, -0.713644, 1.58931);
    points->InsertNextPoint(0.375185, -1.1547, 1.58931);
    points->InsertNextPoint(1.96449, 0, 0.375185);
    points->InsertNextPoint(0.607062, 1.86835, 0.375185);
    points->InsertNextPoint(-1.58931, 1.1547, 0.375185);
    points->InsertNextPoint(-1.58931, -1.1547, 0.375185);
    points->InsertNextPoint(0.607062, -1.86835, 0.375185);
    points->InsertNextPoint(1.58931, 1.1547, -0.375185);
    points->InsertNextPoint(-0.607062, 1.86835, -0.375185);
    points->InsertNextPoint(-1.96449, 0, -0.375185);
    points->InsertNextPoint(-0.607062, -1.86835, -0.375185);
    points->InsertNextPoint(1.58931, -1.1547, -0.375185);
    points->InsertNextPoint(0.982247, 0.713644, -1.58931);
    points->InsertNextPoint(-0.375185, 1.1547, -1.58931);
    points->InsertNextPoint(-1.21412, 0, -1.58931);
    points->InsertNextPoint(-0.375185, -1.1547, -1.58931);
    points->InsertNextPoint(0.982247, -0.713644, -1.58931);

    vtkIdType dodechedronPointsIds[20] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
                                          10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

    vtkIdType dodechedronFace[12][5] = {
                                        {0, 1, 2, 3, 4},     {0, 5, 10, 6, 1},    {1, 6, 11, 7, 2},
                                        {2, 7, 12, 8, 3},    {3, 8, 13, 9, 4},    {4, 9, 14, 5, 0},
                                        {15, 10, 5, 14, 19}, {16, 11, 6, 10, 15}, {17, 12, 7, 11, 16},
                                        {18, 13, 8, 12, 17}, {19, 14, 9, 13, 18}, {19, 18, 17, 16, 15}};
    auto dodechedron_faces = vtkSmartPointer<vtkIdList>::New();
    for (int i = 0; i < cnt_faces; ++i) {
        dodechedron_faces->InsertNextId(cnt_face_vertices);
        for (int j = 0; j < cnt_face_vertices; ++j) {
            dodechedron_faces->InsertNextId(dodechedronFace[i][j]);
        }
    }
    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->InsertNextCell(
        VTK_POLYHEDRON,         // 类型：表示这是一个多面体
        cnt_vertices,           // 一共多少个点
        dodechedronPointsIds,   // 所有点的 ID（顺序无所谓，但要包含所有）
        cnt_faces,              // 面的数量
        dodechedron_faces->GetPointer(0) // 所有面的数据
        );
    ug->SetPoints(points);
    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakePyramid() {
    int cnt_vertices = 5;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(1.0, 1.0, 0.0);
    points->InsertNextPoint(-1.0, 1.0, 0.0);
    points->InsertNextPoint(-1.0, -1.0, 0.0);
    points->InsertNextPoint(1.0, -1.0, 0.0);
    points->InsertNextPoint(0.0, 0.0, 1.0);

    auto pyramid = vtkSmartPointer<vtkPyramid>::New();
    for (int i = 0; i < cnt_vertices; ++i) {
        pyramid->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->InsertNextCell(pyramid->GetCellType(), pyramid->GetPointIds());
    ug->SetPoints(points);

    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakeTetrahedron() {
    int cnt_vertices = 4;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 0, 0);
    points->InsertNextPoint(1, 1, 0);
    points->InsertNextPoint(0, 1, 1);

    auto tetra = vtkSmartPointer<vtkTetra>::New();
    for (int i = 0; i < cnt_vertices; ++i) {
        tetra->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->InsertNextCell(tetra->GetCellType(), tetra->GetPointIds());
    ug->SetPoints(points);

    return ug;
}

// 两个四边形
vtkSmartPointer<vtkUnstructuredGrid> MakeVoxel() {
    auto cnt_vertices = 8;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 0, 0);
    points->InsertNextPoint(0, 1, 0);
    points->InsertNextPoint(1, 1, 0);

    points->InsertNextPoint(0, 0, 1);
    points->InsertNextPoint(1, 0, 1);
    points->InsertNextPoint(0, 1, 1);
    points->InsertNextPoint(1, 1, 1);

    auto voxel = vtkSmartPointer<vtkVoxel>::New();
    for (int i = 0; i < cnt_vertices; ++i) {
        voxel->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->InsertNextCell(voxel->GetCellType(), voxel->GetPointIds());
    ug->SetPoints(points);
    return ug;
}

// 两个三角形
vtkSmartPointer<vtkUnstructuredGrid> MakeWedge() {
    int cnt_vertices = 6;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 1, 0);
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(0, .5, .5);

    points->InsertNextPoint(1, 1, 0);
    points->InsertNextPoint(1, 0.0, 0.0);
    points->InsertNextPoint(1, .5, .5);

    auto wedge = vtkSmartPointer<vtkWedge>::New();
    for (int i = 0; i < cnt_vertices; ++i) {
        wedge->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->InsertNextCell(wedge->GetCellType(), wedge->GetPointIds());
    ug->SetPoints(points);

    return ug;
}
}

