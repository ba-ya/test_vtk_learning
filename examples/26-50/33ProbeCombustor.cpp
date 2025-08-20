#include "00headers.h"

#include <vtkAppendPolyData.h>
#include <vtkContourFilter.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkMultiBlockPLOT3DReader.h>
#include <vtkNamedColors.h>
#include <vtkOutLineFilter.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProbeFilter.h>
#include <vtkProperty.h>
#include <vtkStructuredGrid.h>
#include <vtkStructuredGridOutlineFilter.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

void ProbeCombustor3d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto pl3d = vtkSmartPointer<vtkMultiBlockPLOT3DReader>::New();
    pl3d->SetXYZFileName(Helper::path("combxyz.bin"));
    pl3d->SetQFileName(Helper::path("combq.bin"));
    pl3d->SetScalarFunctionNumber(100);
    pl3d->SetVectorFunctionNumber(202);
    pl3d->Update();

    auto sg = dynamic_cast<vtkStructuredGrid *>(pl3d->GetOutput()->GetBlock(0));

    // plane
    auto plane = vtkSmartPointer<vtkPlaneSource>::New();
    plane->SetResolution(50, 50);

    std::vector<std::array<double, 3>> translate_pos = {
                                           {3.7, 0.0, 28.37},
                                           {9.2, 0.0, 31.20},
                                           {13.27, 0.0, 33.30},
                                           };
    std::vector<vtkSmartPointer<vtkTransformPolyDataFilter>> tpds;
    for (int i = 0; i < 3; ++i) {
        auto transform = vtkSmartPointer<vtkTransform>::New();
        transform->Translate(translate_pos[i][0], translate_pos[i][1], translate_pos[i][2]);
        transform->Scale(5, 5, 5);
        transform->RotateY(90);

        auto tpd = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        tpd->SetInputConnection(plane->GetOutputPort());
        tpd->SetTransform(transform);
        tpds.push_back(tpd);
    }

    auto append_f = vtkSmartPointer<vtkAppendPolyData>::New();
    for (auto &tpd : tpds) {
        append_f->AddInputConnection(tpd->GetOutputPort());
    }

    auto probe = vtkSmartPointer<vtkProbeFilter>::New();
    probe->SetInputConnection(append_f->GetOutputPort());
    probe->SetSourceData(sg);

    auto contour = vtkSmartPointer<vtkContourFilter>::New();
    contour->SetInputConnection(probe->GetOutputPort());
    contour->GenerateValues(100, sg->GetScalarRange());

    auto contour_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    contour_mapper->SetInputConnection(contour->GetOutputPort());
    contour_mapper->SetScalarRange(sg->GetScalarRange());

    auto plane_actor = vtkSmartPointer<vtkActor>::New();
    plane_actor->SetMapper(contour_mapper);

    // file outline
    auto out_line = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
    out_line->SetInputData(sg);
    auto out_line_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    out_line_mapper->SetInputConnection(out_line->GetOutputPort());
    auto out_line_actor = vtkSmartPointer<vtkActor>::New();
    out_line_actor->SetMapper(out_line_mapper);
    out_line_actor->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());
    out_line_actor->GetProperty()->SetLineWidth(2);

    // plane outline
    std::vector<vtkSmartPointer<vtkActor>> tpd_out_line_actors;
    for (auto &tpd : tpds) {
        auto tpd_out_line = vtkSmartPointer<vtkOutlineFilter>::New();
        tpd_out_line->SetInputConnection(tpd->GetOutputPort());

        auto tpd_out_line_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        tpd_out_line_mapper->SetInputConnection(tpd_out_line->GetOutputPort());

        auto tpd_out_line_actor = vtkSmartPointer<vtkActor>::New();
        tpd_out_line_actor->SetMapper(tpd_out_line_mapper);
        tpd_out_line_actor->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());
        tpd_out_line_actor->GetProperty()->SetLineWidth(2);
        tpd_out_line_actors.push_back(tpd_out_line_actor);
    }

    renderer->AddActor(plane_actor);
    renderer->AddActor(out_line_actor);
    for (auto &a : tpd_out_line_actors) {
        renderer->AddActor(a);
    }
}
