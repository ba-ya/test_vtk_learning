#include "00headers.h"

#include <vtkBoxMuellerRandomSequence.h>
#include <vtkDoubleArray.h>
#include <vtkGlyph3DMapper.h>
#include <vtkLabeledDataMapper.h>
#include <vtkLine.h>
#include <vtkNamedColors.h>
#include <vtkPCAStatistics.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkSphereSource.h>
#include <vtkTable.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkVertexGlyphFilter.h>

void PCADemo2d::Draw(std::vector<vtkSmartPointer<vtkRenderer>> renderers)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto random_sequence = vtkSmartPointer<vtkBoxMuellerRandomSequence>::New();
    auto points = vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i < 200; ++i) {
        auto x = random_sequence->GetNextScaledValue(0, 2);
        auto y = random_sequence->GetNextScaledValue(0, 5);
        points->InsertNextPoint(x, y, 0);
    }
    auto polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);

    auto transform = vtkSmartPointer<vtkTransform>::New();
    transform->RotateZ(90);

    auto transform_filter = vtkSmartPointer<vtkTransformFilter>::New();
    transform_filter->SetTransform(transform);
    transform_filter->SetInputData(polydata);
    transform_filter->Update();

    // renderer1, points
    auto sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetThetaResolution(21);
    sphere->SetPhiResolution(21);
    sphere->SetRadius(.2);
    sphere->Update();

    auto glyph_mapper = vtkSmartPointer<vtkGlyph3DMapper>::New();
    glyph_mapper->SetInputData(transform_filter->GetOutput());
    glyph_mapper->SetSourceConnection(sphere->GetOutputPort());

    auto glyph_actor = vtkSmartPointer<vtkActor>::New();
    glyph_actor->SetMapper(glyph_mapper);
    glyph_actor->GetProperty()->SetColor(colors->GetColor3d("LightGoldenrodYellow").GetData());
    glyph_actor->GetProperty()->SetOpacity(.5);

    auto label_mapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
    label_mapper->SetInputData(transform_filter->GetOutput());

    auto label_actor = vtkSmartPointer<vtkActor2D>::New();
    label_actor->SetMapper(label_mapper);
    label_actor->GetProperty()->SetColor(colors->GetColor3d("DeepPink").GetData());;

    // renderer2, project points
    auto xarray = vtkSmartPointer<vtkDoubleArray>::New();
    xarray->SetNumberOfComponents(1);
    xarray->SetName("x");
    auto yarray = vtkSmartPointer<vtkDoubleArray>::New();
    yarray->SetNumberOfComponents(1);
    yarray->SetName("y");

    for (int i = 0; i < polydata->GetNumberOfPoints(); ++i) {
        double p[3];
        transform_filter->GetOutput()->GetPoint(i, p);
        xarray->InsertNextValue(p[0]);
        yarray->InsertNextValue(p[1]);
    }

    auto dataset_table = vtkSmartPointer<vtkTable>::New();
    dataset_table->AddColumn(xarray);
    dataset_table->AddColumn(yarray);

    auto pca_statistics = vtkSmartPointer<vtkPCAStatistics>::New();
    pca_statistics->SetInputData(vtkStatisticsAlgorithm::INPUT_DATA, dataset_table);
    pca_statistics->SetColumnStatus("x", 1);
    pca_statistics->SetColumnStatus("y", 1);
    pca_statistics->RequestSelectedColumns();
    pca_statistics->SetDeriveOption(true);
    pca_statistics->Update();

    // 特征值
    auto eigen_values = vtkSmartPointer<vtkDoubleArray>::New();
    pca_statistics->GetEigenvalues(eigen_values);
    auto eigen_vectors = vtkSmartPointer<vtkDoubleArray>::New();
    pca_statistics->GetEigenvectors(eigen_vectors);

    auto evec1 = vtkSmartPointer<vtkDoubleArray>::New();
    pca_statistics->GetEigenvector(0, evec1);

    double p0[3];
    p0[0] = -100 * evec1->GetValue(0);
    p0[1] = -100 * evec1->GetValue(1);
    p0[2] = 0;
    double p1[3];
    p1[0] = 100 * evec1->GetValue(0);
    p1[1] = 100 * evec1->GetValue(1);
    p1[2] = 0;

    auto project_points = vtkSmartPointer<vtkPoints>::New();
    for (auto i = 0; i < polydata->GetNumberOfPoints(); ++i) {
        double p[3];
        transform_filter->GetOutput()->GetPoint(i, p);
        double t, closest_point[3];
        vtkLine::DistanceToLine(p, p0, p1, t, closest_point);
        project_points->InsertNextPoint(t, 0, 0);
    }
    auto project_polydata = vtkSmartPointer<vtkPolyData>::New();
    project_polydata->SetPoints(project_points);

    auto project_glyph_filter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    project_glyph_filter->SetInputData(project_polydata);
    project_glyph_filter->Update();

    auto project_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    project_mapper->SetInputConnection(project_glyph_filter->GetOutputPort());
    auto project_actor = vtkSmartPointer<vtkActor>::New();
    project_actor->SetMapper(project_mapper);
    project_actor->GetProperty()->SetPointSize(3);
    project_actor->GetProperty()->SetColor(colors->GetColor3d("Gold").GetData());

    renderers[0]->AddActor(glyph_actor);
    // renderers[0]->AddActor2D(label_actor);

    renderers[1]->AddActor(project_actor);
}
