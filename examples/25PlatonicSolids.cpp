#include "00headers.h"

#include <vtkLookupTable.h>
#include <vtkNamedColors.h>
#include <vtkPlatonicSolidSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>

namespace {
vtkSmartPointer<vtkLookupTable> get_platonic_LUT();
}

void PlatonicSolids3d::Draw(std::vector<vtkSmartPointer<vtkRenderer>> renderers)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto text_property = vtkSmartPointer<vtkTextProperty>::New();
    text_property->SetFontSize(16);
    text_property->SetJustificationToCentered();

    QStringList name_orientation;
    name_orientation << "TETRAHEDRON" << "CUBE" << "OCTAHEDRON" << "ICOSAHEDRON" << "DODECAHEDRON";
    auto lut = get_platonic_LUT();

    for (int i = 0; i < name_orientation.count(); ++i) {
        auto platonic_solid = vtkSmartPointer<vtkPlatonicSolidSource>::New();
        platonic_solid->SetSolidType(i);

        auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(platonic_solid->GetOutputPort());
        mapper->SetLookupTable(lut);
        mapper->SetScalarRange(0, 19);

        auto actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);

        auto text_mapper = vtkSmartPointer<vtkTextMapper>::New();
        text_mapper->SetInput(name_orientation.at(i).toUtf8().constData());
        text_mapper->SetTextProperty(text_property);

        auto text_actor = vtkSmartPointer<vtkActor2D>::New();
        text_actor->SetMapper(text_mapper);
        text_actor->SetPosition(120, 16);

        renderers[i]->AddActor(actor);
        renderers[i]->AddActor2D(text_actor);
    }
}

namespace {
vtkSmartPointer<vtkLookupTable> get_platonic_LUT() {
    auto lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfTableValues(20);
    lut->SetNumberOfTableValues(20);
    lut->SetTableRange(0.0, 19.0);
    lut->Build();
    lut->SetTableValue(0, 0.1, 0.1, 0.1);
    lut->SetTableValue(1, 0, 0, 1);
    lut->SetTableValue(2, 0, 1, 0);
    lut->SetTableValue(3, 0, 1, 1);
    lut->SetTableValue(4, 1, 0, 0);
    lut->SetTableValue(5, 1, 0, 1);
    lut->SetTableValue(6, 1, 1, 0);
    lut->SetTableValue(7, 0.9, 0.7, 0.9);
    lut->SetTableValue(8, 0.5, 0.5, 0.5);
    lut->SetTableValue(9, 0.0, 0.0, 0.7);
    lut->SetTableValue(10, 0.5, 0.7, 0.5);
    lut->SetTableValue(11, 0, 0.7, 0.7);
    lut->SetTableValue(12, 0.7, 0, 0);
    lut->SetTableValue(13, 0.7, 0, 0.7);
    lut->SetTableValue(14, 0.7, 0.7, 0);
    lut->SetTableValue(15, 0, 0, 0.4);
    lut->SetTableValue(16, 0, 0.4, 0);
    lut->SetTableValue(17, 0, 0.4, 0.4);
    lut->SetTableValue(18, 0.4, 0, 0);
    lut->SetTableValue(19, 0.4, 0, 0.4);

    return lut;
}
}
