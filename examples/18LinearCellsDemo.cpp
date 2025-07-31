#include "00headers.h"

#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkGlyph3DMapper.h>
#include <vtkHexagonalPrism.h>
#include <vtkHexahedron.h>
#include <vtkLabeledDataMApper.h>
#include <vtkLightKit.h>
#include <vtkLine.h>
#include <vtkNamedColors.h>
#include <vtkPentagonalPrism.h>
#include <vtkPixel.h>
#include <vtkPolyLine.h>
#include <vtkPolyVertex.h>
#include <vtkPolygon.h>
#include <vtkProperty.h>
#include <vtkPyramid.h>
#include <vtkQuad.h>
#include <vtkSphereSource.h>
#include <vtkTetra.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkTriangle.h>
#include <vtkTriangleStrip.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVertex.h>
#include <vtkVoxel.h>
#include <vtkWedge.h>

using cellProp =
    std::tuple<QString, vtkSmartPointer<vtkUnstructuredGrid>>;

namespace {
QList<cellProp> GetUnstructuredGrids();

vtkSmartPointer<vtkUnstructuredGrid> MakeVertex();
vtkSmartPointer<vtkUnstructuredGrid> MakePolyVertex();
vtkSmartPointer<vtkUnstructuredGrid> MakeLine();
vtkSmartPointer<vtkUnstructuredGrid> MakePolyLine();
vtkSmartPointer<vtkUnstructuredGrid> MakeTriangle();
vtkSmartPointer<vtkUnstructuredGrid> MakeTriangleStrip();
vtkSmartPointer<vtkUnstructuredGrid> MakePolygon();
vtkSmartPointer<vtkUnstructuredGrid> MakePixel();
vtkSmartPointer<vtkUnstructuredGrid> MakeQuad();
vtkSmartPointer<vtkUnstructuredGrid> MakeTetra();
vtkSmartPointer<vtkUnstructuredGrid> MakeVoxel();
vtkSmartPointer<vtkUnstructuredGrid> MakeHexahedron();
vtkSmartPointer<vtkUnstructuredGrid> MakeWedge();
vtkSmartPointer<vtkUnstructuredGrid> MakePyramid();
vtkSmartPointer<vtkUnstructuredGrid> MakePentagonalPrism();
vtkSmartPointer<vtkUnstructuredGrid> MakeHexagonalPrism();

vtkNew<vtkTextProperty> GetTextProperty();
vtkNew<vtkTextProperty> GetLabelProperty();
vtkNew<vtkProperty> GetActorProperty();

} // namespace

void LinearCellsDemo3d::Draw(std::vector<vtkSmartPointer<vtkRenderer>> renderers)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetRadius(0.04);

    auto cells = GetUnstructuredGrids();
    auto actor_property = GetActorProperty();
    auto text_property = GetTextProperty();
    auto label_property = GetLabelProperty();
    for (int i = 0; i < cells.size(); ++i) {
        auto &[name, ug] = cells[i];
        // ug
        auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        mapper->SetInputData(ug);
        auto actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->SetProperty(actor_property);
        // point
        auto point_mapper = vtkSmartPointer<vtkGlyph3DMapper>::New();
        point_mapper->SetInputData(ug);
        point_mapper->SetSourceConnection(sphere->GetOutputPort());
        point_mapper->ScalingOff();
        point_mapper->ScalarVisibilityOff();
        auto point_actor = vtkSmartPointer<vtkActor>::New();
        point_actor->SetMapper(point_mapper);
        point_actor->GetProperty()->SetColor(colors->GetColor3d("Gold").GetData());
        // text
        auto text_mapper = vtkSmartPointer<vtkTextMapper>::New();
        text_mapper->SetInput((name + name).toUtf8().constData());
        text_mapper->SetTextProperty(label_property);
        auto text_actor = vtkSmartPointer<vtkActor2D>::New();
        text_actor->SetMapper(text_mapper);
        //label
        auto label_mapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
        label_mapper->SetInputData(ug);
        label_mapper->SetLabelTextProperty(label_property);
        auto label_actor = vtkSmartPointer<vtkActor2D>::New();
        label_actor->SetMapper(label_mapper);

        renderers[i]->AddActor(actor);
        renderers[i]->AddActor(point_actor);
        renderers[i]->AddActor2D(text_actor);
        renderers[i]->AddActor2D(label_actor);
    }
}

namespace {
QList<cellProp> GetUnstructuredGrids() {
    return {
            {cellProp("VTK_VERTEX", MakeVertex())},
            {cellProp("VTK_POLY_VERTEX", MakePolyVertex())},
            {cellProp("VTK_LINE", MakeLine())},
            {cellProp("VTK_POLY_LINE", MakePolyLine())},
            {cellProp("VTK_TRIANGLE", MakeTriangle())},
            {cellProp("VTK_TRIANGLE_STRIP", MakeTriangleStrip())},
            {cellProp("VTK_POLYGON", MakePolygon())},
            {cellProp("VTK_PIXEL", MakePixel())},
            {cellProp("VTK_QUAD", MakeQuad())},
            {cellProp("VTK_TETRA", MakeTetra())},
            {cellProp("VTK_VOXEL", MakeVoxel())},
            {cellProp("VTK_HEXAHEDRON", MakeHexahedron())},
            {cellProp("VTK_WEDGE", MakeWedge())},
            {cellProp("VTK_PYRAMID", MakePyramid())},
            {cellProp("VTK_PENTAGONAL_PRISM", MakePentagonalPrism())},
            {cellProp("VTK_HEXAGONAL_PRISM", MakeHexagonalPrism())},
            };
}

vtkSmartPointer<vtkUnstructuredGrid> MakeVertex() {
    auto cnt_vertices = 1;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0,0,0);

    auto vertex = vtkSmartPointer<vtkVertex>::New();
    for (int i = 0; i < cnt_vertices; ++i) {
        vertex->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(points);
    ug->InsertNextCell(vertex->GetCellType(), vertex->GetPointIds());
    return ug;
}
vtkSmartPointer<vtkUnstructuredGrid> MakePolyVertex() {
    auto cnt_vertices = 6;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 0, 0);
    points->InsertNextPoint(0, 1, 0);
    points->InsertNextPoint(0, 0, 1);
    points->InsertNextPoint(1, 0, 0.4);
    points->InsertNextPoint(0, 1, 0.6);

    auto poly_vertex = vtkSmartPointer<vtkPolyVertex>::New();
    poly_vertex->GetPointIds()->SetNumberOfIds(cnt_vertices);
    for (int i = 0; i < cnt_vertices; ++i) {
        poly_vertex->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(points);
    ug->InsertNextCell(poly_vertex->GetCellType(), poly_vertex->GetPointIds());
    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakeLine() {
    auto cnt_vertices = 2;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0,0,0);
    points->InsertNextPoint(0.5,0.5,0);

    auto line = vtkSmartPointer<vtkLine>::New();
    line->GetPointIds()->SetNumberOfIds(cnt_vertices);
    for (int i = 0; i < cnt_vertices; ++i) {
        line->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(points);
    ug->InsertNextCell(line->GetCellType(), line->GetPointIds());
    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakePolyLine() {
    auto cnt_vertices = 5;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0.5, 0);
    points->InsertNextPoint(0.5, 0, 0);
    points->InsertNextPoint(1, 0.3, 0);
    points->InsertNextPoint(1.5, 0.4, 0);
    points->InsertNextPoint(2.0, 0.4, 0);

    auto polyline = vtkSmartPointer<vtkPolyLine>::New();
    polyline->GetPointIds()->SetNumberOfIds(cnt_vertices);
    for (int i = 0; i < cnt_vertices; ++i) {
        polyline->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(points);
    ug->InsertNextCell(polyline->GetCellType(), polyline->GetPointIds());
    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakeTriangle() {
    auto cnt_vertices = 3;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(0.5, 0.5, 0);
    points->InsertNextPoint(0.2, 1, 0);

    auto triangle = vtkSmartPointer<vtkTriangle>::New();
    triangle->GetPointIds()->SetNumberOfIds(cnt_vertices);
    for (int i = 0; i < cnt_vertices; ++i) {
        triangle->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(points);
    ug->InsertNextCell(triangle->GetCellType(), triangle->GetPointIds());
    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakeTriangleStrip() {
    auto cnt_vertices = 10;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0,0,0);
    points->InsertNextPoint(1,-0.1,0);
    points->InsertNextPoint(0.5, 1, 0);
    points->InsertNextPoint(2.0, -0.1, 0);
    points->InsertNextPoint(1.5, 0.8, 0);
    points->InsertNextPoint(3.0, 0, 0);
    points->InsertNextPoint(2.5, 0.9, 0);
    points->InsertNextPoint(4.0, -0.2, 0);
    points->InsertNextPoint(3.5, 0.8, 0);
    points->InsertNextPoint(4.5, 1.1, 0);

    auto triangle_strip = vtkSmartPointer<vtkTriangleStrip>::New();
    triangle_strip->GetPointIds()->SetNumberOfIds(cnt_vertices);
    for (int i = 0; i < cnt_vertices; ++i) {
        triangle_strip->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(points);
    ug->InsertNextCell(triangle_strip->GetCellType(), triangle_strip->GetPointIds());
    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakePolygon() {
    auto cnt_vertices = 6;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, -0.1, 0);
    points->InsertNextPoint(0.8, 0.5, 0);
    points->InsertNextPoint(1, 1, 0);
    points->InsertNextPoint(0.6, 1.2, 0);
    points->InsertNextPoint(0, 0.8, 0);

    auto polygon = vtkSmartPointer<vtkPolygon>::New();
    polygon->GetPointIds()->SetNumberOfIds(cnt_vertices);
    for (int i = 0; i < cnt_vertices; ++i){
        polygon->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(points);
    ug->InsertNextCell(polygon->GetCellType(), polygon->GetPointIds());
    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakePixel() {
    auto cnt_vertices = 4;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 0, 0);
    points->InsertNextPoint(0, 1, 0);
    points->InsertNextPoint(1, 1, 0);

    auto pixel = vtkSmartPointer<vtkPixel>::New();
    pixel->GetPointIds()->SetNumberOfIds(cnt_vertices);
    for (int i = 0; i < cnt_vertices; ++i) {
        pixel->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(points);
    ug->InsertNextCell(pixel->GetCellType(), pixel->GetPointIds());
    return ug;
}

// another way to set points
vtkSmartPointer<vtkUnstructuredGrid> MakeQuad() {
    auto cnt_vertices = 4;
    auto quad = vtkSmartPointer<vtkQuad>::New();
    quad->GetPoints()->SetPoint(0, 0, 0, 0);
    quad->GetPoints()->SetPoint(1, 1, 0, 0);
    quad->GetPoints()->SetPoint(2, 1, 1, 0);
    quad->GetPoints()->SetPoint(3, 0, 1, 0);

    for (int i = 0; i < cnt_vertices; ++i) {
        quad->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(quad->GetPoints());
    ug->InsertNextCell(quad->GetCellType(), quad->GetPointIds());
    return ug;
}

// another way to insert cell
vtkSmartPointer<vtkUnstructuredGrid> MakeTetra() {
    auto cnt_vertices = 4;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 0, 0);
    points->InsertNextPoint(1, 0, -1);
    points->InsertNextPoint(0, 1, -1);

    auto tetra = vtkSmartPointer<vtkTetra>::New();
    // SetNumberOfPoints is not a must
    for (int i = 0; i < cnt_vertices; ++i) {
        tetra->GetPointIds()->SetId(i, i);
    }
    qDebug() << "tetra cnt_points:" << tetra->GetPoints()->GetNumberOfPoints(); // 4

    auto cell_array = vtkSmartPointer<vtkCellArray>::New();
    cell_array->InsertNextCell(tetra);

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(points);
    ug->SetCells(tetra->GetCellType(), cell_array);
    return ug;
}

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
    ug->SetPoints(points);
    ug->InsertNextCell(voxel->GetCellType(), voxel->GetPointIds());
    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakeHexahedron() {
    auto cnt_vertices = 8;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0.0, 0.0, 0.0);
    points->InsertNextPoint(1.0, 0.0, 0.0);
    points->InsertNextPoint(1.0, 1.0, 0.0);
    points->InsertNextPoint(0.0, 1.0, 0.0);
    points->InsertNextPoint(0.0, 0.0, 1.0);
    points->InsertNextPoint(1.0, 0.0, 1.0);
    points->InsertNextPoint(1.0, 1.0, 1.0);
    points->InsertNextPoint(0.0, 1.0, 1.0);

    auto hex = vtkSmartPointer<vtkHexahedron>::New();
    for (int i = 0; i < cnt_vertices; ++i) {
        hex->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(points);
    ug->InsertNextCell(hex->GetCellType(), hex->GetPointIds());
    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakeWedge() {
    auto cnt_vertices = 6;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(0, 0, 1);
    points->InsertNextPoint(0, 0.5, 0.5);
    points->InsertNextPoint(1, 0, 0);
    points->InsertNextPoint(1, 0, 1);
    points->InsertNextPoint(1, 0.5, 0.5);

    auto wedge = vtkSmartPointer<vtkWedge>::New();
    for (int i = 0; i < cnt_vertices; ++i) {
        wedge->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(points);
    ug->InsertNextCell(wedge->GetCellType(), wedge->GetPointIds());
    return ug;
}

vtkSmartPointer<vtkUnstructuredGrid> MakePyramid() {
    auto cnt_vertices = 5;
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(1.0, 0.0, -1.0);
    points->InsertNextPoint(-1.0, 0.0, -1.0);
    points->InsertNextPoint(-1.0, 0.0, 1.0);
    points->InsertNextPoint(1.0, 0.0, 1.0);
    points->InsertNextPoint(0.0, 2.0, 0.0);

    auto pyramid = vtkSmartPointer<vtkPyramid>::New();
    for (int i = 0; i < cnt_vertices; ++i) {
        pyramid->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(points);
    ug->InsertNextCell(pyramid->GetCellType(), pyramid->GetPointIds());
    return ug;
}

// 五角棱镜
vtkSmartPointer<vtkUnstructuredGrid> MakePentagonalPrism() {
    auto cnt_vertices = 10;
    auto pentagonal_prism = vtkSmartPointer<vtkPentagonalPrism>::New();
    double scale = 2.0;
    pentagonal_prism->GetPoints()->SetPoint(0, 11 / scale, 10 / scale, 10 / scale);
    pentagonal_prism->GetPoints()->SetPoint(1, 13 / scale, 10 / scale, 10 / scale);
    pentagonal_prism->GetPoints()->SetPoint(2, 14 / scale, 12 / scale, 10 / scale);
    pentagonal_prism->GetPoints()->SetPoint(3, 12 / scale, 14 / scale, 10 / scale);
    pentagonal_prism->GetPoints()->SetPoint(4, 10 / scale, 12 / scale, 10 / scale);
    pentagonal_prism->GetPoints()->SetPoint(5, 11 / scale, 10 / scale, 14 / scale);
    pentagonal_prism->GetPoints()->SetPoint(6, 13 / scale, 10 / scale, 14 / scale);
    pentagonal_prism->GetPoints()->SetPoint(7, 14 / scale, 12 / scale, 14 / scale);
    pentagonal_prism->GetPoints()->SetPoint(8, 12 / scale, 14 / scale, 14 / scale);
    pentagonal_prism->GetPoints()->SetPoint(9, 10 / scale, 12 / scale, 14 / scale);

    for (int i = 0; i < cnt_vertices; ++i) {
        pentagonal_prism->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(pentagonal_prism->GetPoints());
    ug->InsertNextCell(pentagonal_prism->GetCellType(), pentagonal_prism->GetPointIds());
    return ug;
}

// 六边形棱镜
vtkSmartPointer<vtkUnstructuredGrid> MakeHexagonalPrism() {
    auto cnt_vertices = 12;
    auto hexagonal_prism = vtkSmartPointer<vtkHexagonalPrism>::New();
    double scale = 2.0;
    hexagonal_prism->GetPoints()->SetPoint(0, 11 / scale, 10 / scale, 10 / scale);
    hexagonal_prism->GetPoints()->SetPoint(1, 13 / scale, 10 / scale, 10 / scale);
    hexagonal_prism->GetPoints()->SetPoint(2, 14 / scale, 12 / scale, 10 / scale);
    hexagonal_prism->GetPoints()->SetPoint(3, 13 / scale, 14 / scale, 10 / scale);
    hexagonal_prism->GetPoints()->SetPoint(4, 11 / scale, 14 / scale, 10 / scale);
    hexagonal_prism->GetPoints()->SetPoint(5, 10 / scale, 12 / scale, 10 / scale);
    hexagonal_prism->GetPoints()->SetPoint(6, 11 / scale, 10 / scale, 14 / scale);
    hexagonal_prism->GetPoints()->SetPoint(7, 13 / scale, 10 / scale, 14 / scale);
    hexagonal_prism->GetPoints()->SetPoint(8, 14 / scale, 12 / scale, 14 / scale);
    hexagonal_prism->GetPoints()->SetPoint(9, 13 / scale, 14 / scale, 14 / scale);
    hexagonal_prism->GetPoints()->SetPoint(10, 11 / scale, 14 / scale, 14 / scale);
    hexagonal_prism->GetPoints()->SetPoint(11, 10 / scale, 12 / scale, 14 / scale);

    for (int i = 0; i < cnt_vertices; ++i) {
        hexagonal_prism->GetPointIds()->SetId(i, i);
    }

    auto ug = vtkSmartPointer<vtkUnstructuredGrid>::New();
    ug->SetPoints(hexagonal_prism->GetPoints());
    ug->InsertNextCell(hexagonal_prism->GetCellType(), hexagonal_prism->GetPointIds());
    return ug;
}

vtkNew<vtkTextProperty> GetTextProperty()
{
    vtkNew<vtkNamedColors> colors;

    vtkNew<vtkTextProperty> pty;
    pty->BoldOn();
    pty->SetJustificationToCentered();
    pty->SetColor(colors->GetColor3d("Black").GetData());
    return pty;
}

vtkNew<vtkTextProperty> GetLabelProperty()
{
    vtkNew<vtkNamedColors> colors;

    vtkNew<vtkTextProperty> pty;
    pty->BoldOn();
    pty->ShadowOn();
    pty->SetJustificationToCentered();
    pty->SetColor(colors->GetColor3d("DeepPink").GetData());
    return pty;
}

vtkNew<vtkProperty> GetActorProperty()
{
    vtkNew<vtkNamedColors> colors;

    vtkNew<vtkProperty> pty;
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
}
