#include "00headers.h"

#include <vtkBiQuadraticQuad.h>
#include <vtkBiQuadraticQuadraticHexahedron.h>
#include <vtkBiQuadraticQuadraticWedge.h>
#include <vtkBiQuadraticTriangle.h>
#include <vtkCubicLine.h>
#include <vtkDataSetMapper.h>
#include <vtkGlyph3DMapper.h>
#include <vtkLabeledDataMapper.h>
#include <vtkNamedColors.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkQuadraticEdge.h>
#include <vtkQuadraticHexahedron.h>
#include <vtkQuadraticLinearQuad.h>
#include <vtkQuadraticLinearWedge.h>
#include <vtkQuadraticPolygon.h>
#include <vtkQuadraticPyramid.h>
#include <vtkQuadraticQuad.h>
#include <vtkQuadraticTetra.h>
#include <vtkQuadraticTriangle.h>
#include <vtkQuadraticWedge.h>
#include <vtkSphereSource.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <vtkTriQuadraticHexahedron.h>
#include <vtkUnstructuredGrid.h>

using cellProp = std::pair<QString, vtkSmartPointer<vtkUnstructuredGrid>>;

namespace {
QList<cellProp> get_unstructured_grids();
template <typename T>
vtkSmartPointer<vtkUnstructuredGrid> MakeUG(vtkSmartPointer<T> cell);
vtkSmartPointer<vtkUnstructuredGrid> MakeQuadraticPolygon();
vtkSmartPointer<vtkUnstructuredGrid> MakeQuadraticPyramid();

vtkSmartPointer<vtkActor> MakeTile(double const bounds[],
                          double const& expansionFactor = 0.5,
                          double const& thicknessRatio = 0.05,
                          double shiftY = -0.05);

vtkSmartPointer<vtkTextProperty> GetLabelProperty();
vtkSmartPointer<vtkProperty> GetBackFaceProperty();
vtkSmartPointer<vtkProperty> GetPointActorProperty();
vtkSmartPointer<vtkProperty> GetActorProperty();
vtkSmartPointer<vtkProperty> GetTileProperty();
}

void IsoparametricCellsDemo3d::Draw(std::vector<vtkSmartPointer<vtkRenderer>> renderers)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetPhiResolution(21);
    sphere->SetThetaResolution(21);
    sphere->SetRadius(.04);

    auto cells = get_unstructured_grids();
    auto text_property = vtkSmartPointer<vtkTextProperty>::New();
    text_property->BoldOn();
    text_property->SetColor(colors->GetColor3d("Black").GetData());

    auto actor_property = GetActorProperty();
    auto label_property = GetLabelProperty();
    auto point_property = GetPointActorProperty();
    auto tile_property = GetTileProperty();

    int i = 0;
    for (auto it = cells.begin(); it != cells.end(); ++it) {
        auto name = it->first;
        auto ug = it->second;

        // text
        auto text_mapper = vtkSmartPointer<vtkTextMapper>::New();
        text_mapper->SetInput(name.toUtf8().constData());
        text_mapper->SetTextProperty(text_property);

        auto text_actor = vtkSmartPointer<vtkActor2D>::New();
        text_actor->SetMapper(text_mapper);

        auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        mapper->SetInputData(ug);

        // ug
        auto actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->SetProperty(actor_property);

        // label
        auto label_mapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
        label_mapper->SetInputData(ug);
        label_mapper->SetLabelTextProperty(label_property);

        auto label_actor = vtkSmartPointer<vtkActor2D>::New();
        label_actor->SetMapper(label_mapper);

        // glyph the points
        auto point_mapper = vtkSmartPointer<vtkGlyph3DMapper>::New();
        point_mapper->SetInputData(ug);
        point_mapper->SetSourceConnection(sphere->GetOutputPort());
        point_mapper->ScalingOff();
        point_mapper->ScalarVisibilityOff();

        auto point_actor = vtkSmartPointer<vtkActor>::New();
        point_actor->SetMapper(point_mapper);
        point_actor->SetProperty(point_property);

        // tile
        auto tile_actor = MakeTile(ug->GetBounds());
        tile_actor->SetProperty(tile_property);

        renderers[i]->AddActor(text_actor);
        renderers[i]->AddActor(actor);
        renderers[i]->AddActor(label_actor);
        renderers[i]->AddActor(point_actor);
        renderers[i]->AddActor(tile_actor);
        i++;
    }

}

namespace {
QList<cellProp> get_unstructured_grids() {
    QList<cellProp> ugs;
    ugs = { {
            cellProp("VTK_QUADRATIC_EDGE",    MakeUG(vtkSmartPointer<vtkQuadraticEdge>::New())),
            cellProp("VTK_QUADRATIC_TRIANGLE",MakeUG(vtkSmartPointer<vtkQuadraticTriangle>::New())),
            cellProp("VTK_QUADRATIC_QUAD",    MakeUG(vtkSmartPointer<vtkQuadraticQuad>::New())),
            cellProp("VTK_QUADRATIC_POLYGON", MakeQuadraticPolygon()),
            cellProp("VTK_QUADRATIC_TETRA",     MakeUG(vtkSmartPointer<vtkQuadraticTetra>::New())),
            cellProp("VTK_QUADRATIC_HEXAHEDRON",MakeUG(vtkSmartPointer<vtkQuadraticHexahedron>::New())),
            cellProp("VTK_QUADRATIC_WEDGE",     MakeUG(vtkSmartPointer<vtkQuadraticWedge>::New())),
            cellProp("VTK_QUADRATIC_PYRAMID",   MakeQuadraticPyramid()),
            cellProp("VTK_BIQUADRATIC_QUAD",    MakeUG(vtkSmartPointer<vtkBiQuadraticQuad>::New())),
            cellProp("VTK_TRIQUADRATIC_HEXAHEDRON",MakeUG(vtkSmartPointer<vtkTriQuadraticHexahedron>::New())),
            cellProp("VTK_QUADRATIC_LINEAR_QUAD",  MakeUG(vtkSmartPointer<vtkQuadraticLinearQuad>::New())),
            cellProp("VTK_QUADRATIC_LINEAR_WEDGE", MakeUG(vtkSmartPointer<vtkQuadraticLinearWedge>::New())),
            cellProp("VTK_BIQUADRATIC_QUADRATIC_WEDGE",    MakeUG(vtkSmartPointer<vtkBiQuadraticQuadraticWedge>::New())),
            cellProp("VTK_BIQUADRATIC_QUADRATIC_HEXAHEDRON",MakeUG(vtkSmartPointer<vtkBiQuadraticQuadraticHexahedron>::New())),
            cellProp("VTK_BIQUADRATIC_TRIANGLE",MakeUG(vtkSmartPointer<vtkBiQuadraticTriangle>::New())),
            cellProp("VTK_CUBIC_LINE",          MakeUG(vtkSmartPointer<vtkCubicLine>::New())),
        },
    };
    return ugs;
}

template <typename T>
vtkSmartPointer<vtkUnstructuredGrid> MakeUG(vtkSmartPointer<T> cell) {
    double* pcoords = cell->GetParametricCoords();
    for (int i = 0; i < cell->GetNumberOfPoints(); ++i) {
        cell->GetPointIds()->SetId(i, i);
        cell->GetPoints()->SetPoint(i, *(pcoords + 3 * i), *(pcoords + 3 * i + 1), *(pcoords + 3 * i + 2));
    }
    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(cell->GetPoints());
    ug->InsertNextCell(cell->GetCellType(), cell->GetPointIds());
    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakeQuadraticPolygon() {
    auto polygon = vtkSmartPointer<vtkQuadraticPolygon>::New();
    int cnt_points = 8;
    polygon->GetPointIds()->SetNumberOfIds(cnt_points);
    for (int i = 0; i < cnt_points; ++i) {
        polygon->GetPointIds()->SetId(i, i);
    }
    polygon->GetPoints()->SetNumberOfPoints(cnt_points);
    polygon->GetPoints()->SetPoint(0, 0, 0, 0);
    polygon->GetPoints()->SetPoint(1, 2.0, 0, 0);
    polygon->GetPoints()->SetPoint(2, 2.0, 2.0, 0);
    polygon->GetPoints()->SetPoint(3, 0, 2.0, 0);
    polygon->GetPoints()->SetPoint(4, 1.0, 0, 0);
    polygon->GetPoints()->SetPoint(5, 2.0, 1.0, 0);
    polygon->GetPoints()->SetPoint(6, 1.0, 2.0, 0);
    polygon->GetPoints()->SetPoint(5, 3.0, 1.0, 0);

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(polygon->GetPoints());
    ug->InsertNextCell(polygon->GetCellType(), polygon->GetPointIds());
    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakeQuadraticPyramid() {
    // auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    auto ug = MakeUG(vtkSmartPointer<vtkQuadraticPyramid>::New());
    return ug;
}

vtkSmartPointer<vtkActor> MakeTile(double const bounds[],
                                   double const& expansionFactor,
                                   double const& thicknessRatio ,
                                   double shiftY) {
    auto plane_source = vtkSmartPointer<vtkPlaneSource>::New();
    auto xmin = bounds[0]; auto xmax = bounds[1];
    auto ymin = bounds[2]; auto ymax = bounds[3];
    auto zmin = bounds[4]; auto zmax = bounds[5];
    plane_source->SetCenter(0, 0, 0);
    plane_source->SetNormal(0, 1, 0);
    plane_source->SetResolution(2, 3);

    auto mat = vtkSmartPointer<vtkMatrix4x4>::New();
    std::vector<std::array<double, 3>> normals = {
                                                  {(xmax - xmin), 0, 0},
                                                  {0, 1, 0},
                                                  {0, 0, (ymax - ymin)},
                                                  {(xmin + xmax) / 2, ymin, (zmin + zmax) / 2}};
    for (int i = 0; i < 4; ++i) {
        auto normal = normals[i];
        auto ratio = i != 3 ? 1.5 : 1.0;
        mat->SetElement(0, i, ratio * normal[0]);
        mat->SetElement(1, i, ratio * normal[1]);
        mat->SetElement(2, i, ratio * normal[2]);
    }
    mat->SetElement(3, 3, 1);
    auto transfrom = vtkSmartPointer<vtkTransform>::New();
    transfrom->SetMatrix(mat);

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(plane_source->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->SetUserTransform(transfrom);

    return actor;
}

vtkSmartPointer<vtkTextProperty> GetLabelProperty() {
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto pty = vtkSmartPointer<vtkTextProperty>::New();
    pty->BoldOn();
    pty->ShadowOn();
    pty->SetJustificationToCentered();
    pty->SetColor(colors->GetColor3d("DeepPink").GetData());
    return pty;
}

vtkSmartPointer<vtkProperty> GetBackFaceProperty() {
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto pty = vtkSmartPointer<vtkProperty>::New();
    return pty;
}

vtkSmartPointer<vtkProperty> GetPointActorProperty() {
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto pty = vtkSmartPointer<vtkProperty>::New();
    pty->SetAmbientColor(colors->GetColor3d("Gold").GetData());
    pty->SetDiffuseColor(colors->GetColor3d("Yellow").GetData());
    pty->SetSpecularColor(colors->GetColor3d("White").GetData());
    pty->SetSpecular(0.5);
    pty->SetDiffuse(0.7);
    pty->SetAmbient(0.5);
    pty->SetSpecularPower(20.0);
    pty->SetOpacity(1.0);
    return pty;
}

vtkSmartPointer<vtkProperty> GetActorProperty() {
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto pty = vtkSmartPointer<vtkProperty>::New();
    pty->SetAmbientColor(colors->GetColor3d("DarkSalmon").GetData());
    pty->SetDiffuseColor(colors->GetColor3d("Seashell").GetData());
    pty->SetSpecularColor(colors->GetColor3d("White").GetData());
    pty->SetSpecular(0.5);
    pty->SetDiffuse(0.7);
    pty->SetAmbient(0.5);
    pty->SetSpecularPower(20.0);
    pty->SetOpacity(0.8);
    pty->EdgeVisibilityOn();
    pty->SetLineWidth(3);
    return pty;
}

vtkSmartPointer<vtkProperty> GetTileProperty() {
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto pty = vtkSmartPointer<vtkProperty>::New();
    pty->SetAmbientColor(colors->GetColor3d("SteelBlue").GetData());
    pty->SetDiffuseColor(colors->GetColor3d("LightSteelBlue").GetData());
    pty->SetSpecularColor(colors->GetColor3d("White").GetData());
    pty->SetSpecular(0.5);
    pty->SetDiffuse(0.7);
    pty->SetAmbient(0.5);
    pty->SetSpecularPower(20.0);
    pty->SetOpacity(0.8);
    pty->EdgeVisibilityOn();
    pty->SetLineWidth(1);
    return pty;
}
}
