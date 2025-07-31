#include "00headers.h"

#include <vtkCameraOrientationWidget.h>
#include <vtkCellCenters.h>
#include <vtkConeSource.h>
#include <vtkGlyph3D.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>

void ConesOnSphere3d::Draw(vtkRenderer *renderer)
{
    auto sphere_source = vtkSmartPointer<vtkSphereSource>::New();
    sphere_source->SetRadius(.5);
    // 纬线数目,16 - 1
    sphere_source->SetPhiResolution(16);
    // 经线数目, 16
    sphere_source->SetThetaResolution(16);
    sphere_source->LatLongTessellationOn();

    auto cone_source = vtkSmartPointer<vtkConeSource>::New();
    cone_source->SetRadius(.08);
    cone_source->SetHeight(.03);
    cone_source->SetResolution(30);

    auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->ComputeCellNormalsOn();
    normals->SetInputConnection(sphere_source->GetOutputPort());

    auto cell_centers = vtkSmartPointer<vtkCellCenters>::New();
    cell_centers->SetInputConnection(normals->GetOutputPort());

    auto glyph_filter = vtkSmartPointer<vtkGlyph3D>::New();
    glyph_filter->OrientOn();
    glyph_filter->SetVectorModeToUseNormal();
    glyph_filter->SetSourceConnection(cone_source->GetOutputPort());
    glyph_filter->SetInputConnection(cell_centers->GetOutputPort());

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(glyph_filter->GetOutputPort());

    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->GetProperty()->SetColor(colors->GetColor3d("DeepSkyBlue").GetData());
    actor->GetProperty()->SetEdgeColor(colors->GetColor3d("DarkSlateBlue").GetData());
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
}
