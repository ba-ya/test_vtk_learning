#include "00headers.h"

#include <random>
#include <vtkMultiBlockPLOT3DReader.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkTubeFilter.h>

#include <vtkSmartPointer.h>
#include <vtkStructuredGrid.h>
#include <vtkPoints.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkMath.h>
#include <vtkLineSource.h>
#include <vtkProbeFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkXYPlotActor.h>
#include <vtkStructuredGridOutLineFilter.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkDataSetSurfaceFilter.h>

void CreateFakeStructuredGrid(vtkSmartPointer<vtkStructuredGrid> sgrid, int dims[3])
{
    sgrid->SetDimensions(dims);

    auto points = vtkSmartPointer<vtkPoints>::New();

    // 随机扰动生成器
    std::mt19937 rng(12345);  // 固定种子保证可复现
    std::uniform_real_distribution<double> dist(-.5, .5);

    for (int k = 0; k < dims[2]; ++k) {
        for (int j = 0; j < dims[1]; ++j)  {
            for (int i = 0; i < dims[0]; ++i)
            {
                double x = static_cast<double>(i) ;+ dist(rng);
                double y = static_cast<double>(j) ;+ dist(rng);
                double z = static_cast<double>(k) ;+ dist(rng);
                points->InsertNextPoint(x, y, z);
            }
        }
    }
    sgrid->SetPoints(points);

    // 标量场: 一个 3D 波动函数
    std::uniform_real_distribution<double> dist_scalar(-5, 15);
    auto scalars = vtkSmartPointer<vtkDoubleArray>::New();
    scalars->SetName("FakeScalars");
    for (int k = 0; k < dims[2]; ++k) {
        for (int j = 0; j < dims[1]; ++j) {
            for (int i = 0; i < dims[0]; ++i) {
                // double val = sin(i * 0.3) * cos(j * 0.3) * sin(k * 0.2);
                double val = dist_scalar(rng);
                scalars->InsertNextValue(val);
            }
        }
    }
    sgrid->GetPointData()->SetScalars(scalars);

    // 向量场: 一个旋转 + 上升的场
    auto vectors = vtkSmartPointer<vtkDoubleArray>::New();
    vectors->SetNumberOfComponents(3);
    vectors->SetName("FakeVectors");
    for (int k = 0; k < dims[2]; ++k) {
        for (int j = 0; j < dims[1]; ++j)  {
            for (int i = 0; i < dims[0]; ++i) {
                double x = static_cast<double>(i);
                double y = static_cast<double>(j);
                double z = static_cast<double>(k);
                double vx = -y + dist(rng);   // 加点噪声
                double vy = x + dist(rng);
                double vz = 0.5 * z + dist(rng);
                vectors->InsertNextTuple3(vx, vy, vz);
            }
        }
    }
    sgrid->GetPointData()->SetVectors(vectors);
}

void XYPlot::Draw(std::vector<vtkSmartPointer<vtkRenderer>> renderers)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto sgrid = vtkSmartPointer<vtkStructuredGrid>::New();
    int dims[3] = {12, 6, 3};
    CreateFakeStructuredGrid(sgrid, dims);

    /// -----------------3D
    // outline
    auto out_line = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
    out_line->SetInputData(sgrid);
    auto out_line_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    out_line_mapper->SetInputConnection(out_line->GetOutputPort());
    auto out_line_actor = vtkSmartPointer<vtkActor>::New();
    out_line_actor->SetMapper(out_line_mapper);
    out_line_actor->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

    double bounds[6];
    out_line_actor->GetBounds(bounds);

    // surface
    auto surface = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
    surface->SetInputData(sgrid);

    auto surface_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    surface_mapper->SetInputConnection(surface->GetOutputPort());
    surface_mapper->SetScalarRange(sgrid->GetPointData()->GetScalars()->GetRange());

    auto surface_actor = vtkSmartPointer<vtkActor>::New();
    surface_actor->SetMapper(surface_mapper);
    surface_actor->GetProperty()->SetOpacity(.5);

    // probe
    auto line = vtkSmartPointer<vtkLineSource>::New();
    line->SetResolution(30);

    std::vector<vtkSmartPointer<vtkProbeFilter>> probes;
    for (int i = 0; i < 3; ++i) {
        auto transfrom = vtkSmartPointer<vtkTransform>::New();
        transfrom->Identity();
        transfrom->Translate((bounds[0] + bounds[1]) / 4 * (i + 1),
                             (bounds[2] + bounds[3]) / 2,
                             (bounds[5])/*bounds[4] + bounds[5]) / 2*/);
        transfrom->RotateZ(90);
        transfrom->Scale(4, 4, 4);

        auto tf = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        tf->SetTransform(transfrom);
        tf->SetInputConnection(line->GetOutputPort());

        auto probe = vtkSmartPointer<vtkProbeFilter>::New();
        probe->SetInputConnection(tf->GetOutputPort());
        probe->SetSourceData(sgrid);

        probes.push_back(probe);
    }

    auto append_f = vtkSmartPointer<vtkAppendPolyData>::New();
    for (auto &a : probes) {
        append_f->AddInputConnection(a->GetOutputPort());
    }

    auto tuber = vtkSmartPointer<vtkTubeFilter>::New();
    tuber->SetInputConnection(append_f->GetOutputPort());
    tuber->SetRadius(.1);

    auto line_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    line_mapper->SetInputConnection(tuber->GetOutputPort());
    auto line_actor = vtkSmartPointer<vtkActor>::New();
    line_actor->SetMapper(line_mapper);

    renderers[0]->AddActor(line_actor);
    renderers[0]->AddActor(out_line_actor);
    renderers[0]->AddActor(surface_actor);

    /// -----------------2D
    QList<QStringList> titles = {
                                 {"Pressure vs. Arc Length(Zoomed View)", "", "P"},
                                 {"Pressure vs. Normalized Arc Length", "", "P"},
                                 {"Pressure vs. Point Id", "Probe Length", "P"},
                                 };
    std::vector<vtkColor3d> prop_colors = {
        colors->GetColor3d("snow"),
        colors->GetColor3d("Tomato"),
        colors->GetColor3d("Peacock"),
    };
    std::vector<double> y_pos = {.67, .33, 0};
    std::vector<vtkSmartPointer<vtkXYPlotActor>> xyplots;
    for (int i = 0; i < 3; ++i) {
        auto xyplot = vtkSmartPointer<vtkXYPlotActor>::New();
        for (auto &a : probes) {
            xyplot->AddDataSetInputConnection(a->GetOutputPort());
        }
        xyplot->SetTitle(titles[i].at(0).toUtf8().constData());
        xyplot->SetXTitle(titles[i].at(1).toUtf8().constData());
        xyplot->SetYTitle(titles[i].at(2).toUtf8().constData());

        auto tp = vtkSmartPointer<vtkTextProperty>::New();
        tp->SetColor(prop_colors[i].GetData());
        xyplot->GetProperty()->SetColor(prop_colors[i].GetData());
        xyplot->SetTitleTextProperty(tp);
        xyplot->SetAxisTitleTextProperty(tp);
        xyplot->SetAxisLabelTextProperty(tp);

        // PositionCoordinate → 定义 左下角（起点）。
        // Position2Coordinate → 定义 尺寸/右上角（终点）。
        // x = 1 → 宽度占据整个窗口宽度（从左到右）。
        // y = 0.33 → 高度 = 窗口的 33%。
        // z = 0 → 无关。
        // 坐标是归一化的 [0,1]（相对窗口大小）。
        xyplot->GetPositionCoordinate()->SetValue(0, y_pos[i], 0);// 位置坐标(归一化)
        xyplot->GetPosition2Coordinate()->SetValue(1, .33, 0);// 宽度, 高度
        xyplots.push_back(xyplot);
    }
    xyplots[0]->SetXValuesToArcLength();
    xyplots[0]->GetProperty()->SetLineWidth(2);

    xyplots[1]->SetXValuesToNormalizedArcLength();
    xyplots[1]->PlotPointsOn();
    xyplots[1]->PlotLinesOff();
    xyplots[1]->GetProperty()->SetPointSize(5);

    xyplots[2]->SetXValuesToIndex();
    xyplots[2]->PlotPointsOn();
    xyplots[2]->GetProperty()->SetPointSize(3);

    for (auto &a : xyplots) {
        renderers[1]->AddActor2D(a);
    }
}
