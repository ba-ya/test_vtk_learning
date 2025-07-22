#include "00headers.h"

#include <vtkCellData.h>
#include <vtkCellTypeSource.h>
#include <vtkCellTypes.h>
#include <vtkColorSeries.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkShrinkFilter.h>
#include <vtkTessellatorFilter.h>
#include <vtkUnstructuredGrid.h>

void CellTypeSource3d::Draw(vtkRenderer *renderer)
{
    auto source = vtkSmartPointer<vtkCellTypeSource>::New();
    source->SetCellType(VTK_TETRA);// VTK_WEDGE, VTK_VOXEL
    source->Update();

    // adjust points
    auto original_points = source->GetOutput()->GetPoints();
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints(source->GetOutput()->GetNumberOfPoints());
    auto rng = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
    rng->SetSeed(5070);
    for (auto i = 0; i < points->GetNumberOfPoints(); ++i) {
        double perturbation[3] = {0,0,0};
// 标准立方体
#if 0
        for (int j = 0; j < 3; ++j) {
            rng->Next();
            perturbation[j] = rng->GetRangeValue(-0.1, 0.1);
        }
#endif
        double curr_point[3];
        original_points->GetPoint(i, curr_point);
        points->SetPoint(i,
                         curr_point[0] + perturbation[0],
                         curr_point[1] + perturbation[1],
                         curr_point[2] + perturbation[2]);
    }
    source->GetOutput()->SetPoints(points);
    // cell
    int num_cells = source->GetOutput()->GetNumberOfCells();
    auto id_array = vtkSmartPointer<vtkIntArray>::New();
    id_array->SetNumberOfTuples(num_cells);
    for (int i = 0; i < num_cells; ++i) {
        id_array->InsertTuple1(i, i + 1);
    }
    id_array->SetName("Ids");
    source->GetOutput()->GetCellData()->AddArray(id_array);
    source->GetOutput()->GetCellData()->SetActiveScalars("Ids");
    // shrink
    auto shrink = vtkSmartPointer<vtkShrinkFilter>::New();
    shrink->SetInputConnection(source->GetOutputPort());
    shrink->SetShrinkFactor(.8);
    // tessellate, 镶嵌
    auto tessellate = vtkSmartPointer<vtkTessellatorFilter>::New();
    tessellate->SetInputConnection(shrink->GetOutputPort());
    // tessellate, 设置最大细分数
    tessellate->SetMaximumNumberOfSubdivisions(3);
    // lut
    auto lut = vtkSmartPointer<vtkLookupTable>::New();
    auto color_series = vtkSmartPointer<vtkColorSeries>::New();
    int series_enum = color_series->BREWER_QUALITATIVE_SET3;
    color_series->SetColorScheme(series_enum);
    color_series->BuildLookupTable(lut, color_series->ORDINAL);

    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputConnection(source->GetOutputPort());
    mapper->SetInputConnection(shrink->GetOutputPort());
    mapper->SetScalarRange(0, num_cells + 1);
    mapper->SetLookupTable(lut);
    mapper->SetScalarModeToUseCellData();
    mapper->SetResolveCoincidentTopologyToPolygonOffset();
    if (source->GetCellType() == VTK_QUADRATIC_PYRAMID ||
        source->GetCellType() == VTK_QUADRATIC_WEDGE) {
        mapper->SetInputConnection(shrink->GetOutputPort());
    } else {
        mapper->SetInputConnection(tessellate->GetOutputPort());
    }

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->EdgeVisibilityOn();

    renderer->AddActor(actor);
}
