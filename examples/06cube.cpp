#include "00headers.h"
#include <vtkFloatArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPointData.h>

void Cube3d::Draw(vtkRenderer *renderer)
{
    auto cube = vtkSmartPointer<vtkPolyData>::New();
    auto points = vtkSmartPointer<vtkPoints>::New(); // 8个顶点
    auto polys = vtkSmartPointer<vtkCellArray>::New(); // 6个面
    auto scalars = vtkSmartPointer<vtkFloatArray>::New(); // 颜色

    // 8个顶点
    std::vector<double> pts = {
        0, 0, 0,
        1, 0, 0,
        1, 1, 0,
        0, 1, 0,
        0, 0, 1,
        1, 0, 1,
        1, 1, 1,
        0, 1, 1,
    };
    for (int i = 0; i < 8; ++i) {
        auto id = i * 3;
        points->InsertPoint(i, pts[id], pts[id + 1], pts[id + 2]);
        scalars->InsertTuple1(i, i); // 前一个i表示位置, 后一个i表示用的是 xx 里第几个数据
    }
    // 6个面
    // 8个点的排布,第一个是xy平面的点,第二个是z=1平面上的4个点
    // |0 3| |4 7|
    // |1 2| |5 6|
    std::vector<vtkIdType> ordering = {
        4,7,6,5, // 上
        0,3,2,1, // 下
        0,1,5,4, // 左
        3,2,6,7, // 右
        1,2,6,5, // 前
        0,3,7,4, // 后
    };
    for (int i = 0; i < 6; ++i) {
        auto id = i * 4;
        std::array<vtkIdType, 4> face = {ordering[id],
                                         ordering[id + 1],
                                         ordering[id + 2],
                                         ordering[id + 3]};
        polys->InsertNextCell(vtkIdType(4), face.data());
    }

    cube->SetPoints(points);
    cube->SetPolys(polys);
    cube->GetPointData()->SetScalars(scalars);

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(cube);
    mapper->SetScalarRange(cube->GetScalarRange());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
}
