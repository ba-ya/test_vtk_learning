#include "00headers.h"

#include <vtkColorTransferFunction.h>
#include <vtkContourFilter.h>
#include <vtkFloatArray.h>
#include <vtkNamedColors.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkShepardMethod.h>
#include <vtkUnsignedCharArray.h>
#include <vtkVertexGlyphFilter.h>

void ShepardMethod3d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(-1, 0, 0);
    points->InsertNextPoint(1, 0, 0);

    unsigned char white[3] = {255, 255, 255};
    unsigned char black[3] = {0, 0, 0};
    auto vetex_colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    vetex_colors->SetNumberOfComponents(3);
    vetex_colors->SetName("Colors");
    vetex_colors->InsertNextTypedTuple(black);
    vetex_colors->InsertNextTypedTuple(white);

    // vertexs
    auto polydata_to_visualize = vtkSmartPointer<vtkPolyData>::New();
    polydata_to_visualize->SetPoints(points);
    polydata_to_visualize->GetPointData()->SetScalars(vetex_colors);

    auto vertex_glyph_filter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertex_glyph_filter->AddInputData(polydata_to_visualize);
    vertex_glyph_filter->Update();

    auto vertex_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vertex_mapper->SetInputConnection(vertex_glyph_filter->GetOutputPort());

    auto vertex_actor = vtkSmartPointer<vtkActor>::New();
    vertex_actor->SetMapper(vertex_mapper);
    vertex_actor->GetProperty()->SetPointSize(5);

    // shepard
    auto values = vtkSmartPointer<vtkFloatArray>::New();
    values->SetNumberOfComponents(1);
    values->SetName("Values");
    values->InsertNextValue(0);
    values->InsertNextValue(1);

    auto polydata_to_process = vtkSmartPointer<vtkPolyData>::New();
    polydata_to_process->SetPoints(points);
    polydata_to_process->GetPointData()->SetScalars(values);

    auto shepard = vtkSmartPointer<vtkShepardMethod>::New();
    shepard->SetInputData(polydata_to_process);
    shepard->SetSampleDimensions(2, 2, 2);
    shepard->SetModelBounds(-1, 1, -.5, .5, -.5, .5);
    shepard->SetMaximumDistance(1);

    // 轮廓
    auto contour_filter = vtkSmartPointer<vtkContourFilter>::New();
    int cnt_contours = 8;
    contour_filter->SetNumberOfContours(cnt_contours);
    for (int i = 0; i < cnt_contours; ++i) {
        contour_filter->SetValue(i, 1.0 / (cnt_contours + 1) * (i + 1));
    }
    contour_filter->SetInputConnection(shepard->GetOutputPort());
    contour_filter->Update();

    auto contour_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    contour_mapper->SetInputConnection(contour_filter->GetOutputPort());
    contour_mapper->ScalarVisibilityOn();
    contour_mapper->SetColorModeToMapScalars();

    double *range = contour_filter->GetOutput()->GetScalarRange();
    auto lut = vtkSmartPointer<vtkColorTransferFunction>::New();
    lut->SetColorSpaceToRGB();
    lut->AddRGBPoint(range[0], 0, 0, 0); // black
    lut->AddRGBPoint(range[1], 1, 1, 1); // white
    lut->SetScaleToLinear();
    contour_mapper->SetLookupTable(lut);

    auto contour_actor = vtkSmartPointer<vtkActor>::New();
    contour_actor->SetMapper(contour_mapper);

    renderer->AddActor(vertex_actor);
    renderer->AddActor(contour_actor);
}
