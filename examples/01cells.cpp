#include "00headers.h"
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkFloatArray.h>
#include <vtkLookupTable.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkQuad.h>

void ExampleCells2d::Draw(vtkRenderer *renderer)
{
    int nx = 10;
    int ny = 10;
    float dx = 1.0;
    float dy = 1.0;

    // 几何顶点
    auto points = vtkSmartPointer<vtkPoints>::New();
    // 单元格
    auto cells = vtkSmartPointer<vtkCellArray>::New();
    // 待映射的颜色值
    auto scalars = vtkSmartPointer<vtkFloatArray>::New();
    scalars->SetName("color values");

    int max = 100;

    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            auto x = i * dx;
            auto y = j * dy;

            vtkIdType ids[4];
            ids[0] = points->InsertNextPoint(x, y, 0); // 左下
            ids[1] = points->InsertNextPoint(x + dx, y, 0); // 右下
            ids[2] = points->InsertNextPoint(x + dx, y + dy, 0); // 右上
            ids[3] = points->InsertNextPoint(x, y + dy, 0); // 左上

            auto quad = vtkSmartPointer<vtkQuad>::New();
            for (int k = 0; k < 4; ++k) {
                quad->GetPointIds()->SetId(k, ids[k]);
            }

            cells->InsertNextCell(quad);
            scalars->InsertNextValue(j * 10 + i); // std::rand() % max
        }
    }

    auto poly_data = vtkSmartPointer<vtkPolyData>::New();
    poly_data->SetPoints(points);
    poly_data->SetPolys(cells);
    poly_data->GetCellData()->SetScalars(scalars);// 每个符号的颜色将根据其数据值

    auto lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfTableValues(max);
    lut->SetRange(0, max);
    lut->Build();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(poly_data);
    mapper->SetLookupTable(lut);
    mapper->SetScalarModeToUseCellData();
    mapper->SetScalarRange(0, max);

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
}
