#include "00headers.h"

#include <vtkConeSource.h>
#include <vtkCylinderSource.h>
#include <vtkDiscretizableColorTransferFunction.h>
#include <vtkElevationFilter.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>

namespace {
vtkSmartPointer<vtkDiscretizableColorTransferFunction> GetCTF();

}

void ColorMapToLUT3d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    auto cone = vtkSmartPointer<vtkConeSource>::New();
    cone->SetResolution(6);
    cone->SetDirection(0, 1, 0);
    cone->SetHeight(1);
    cone->Update();

    auto cylinder = vtkSmartPointer<vtkCylinderSource>::New();
    cylinder->SetResolution(21);
    cylinder->SetHeight(.5);
    cylinder->Update();

    double bounds[6];
    cylinder->GetOutput()->GetBounds(bounds);

    // 根据点在某一方向上的高度（投影）生成标量值，然后这些标量值就可以用于颜色映射。
    // 某一方向是, LowPoint->HighPoint
    auto elevation_filter = vtkSmartPointer<vtkElevationFilter>::New();
    elevation_filter->SetLowPoint(0, bounds[2], 0);
    elevation_filter->SetHighPoint(0, bounds[3], 0);
    elevation_filter->SetInputConnection(cylinder->GetOutputPort());

    // vtkSmartPointer<vtkDiscretizableColorTransferFunction>
    auto ctf = GetCTF();

    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(elevation_filter->GetOutputPort());
    mapper->SetLookupTable(ctf);
    // 使用标量值去查表映射颜色，而不是直接把标量值当颜色。
    mapper->SetColorModeToMapScalars();
    // 如果开启, 标量会先在顶点间插值，再进行颜色映射。
    mapper->InterpolateScalarsBeforeMappingOn();

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
}

namespace {
vtkSmartPointer<vtkDiscretizableColorTransferFunction> GetCTF() {
    auto ctf = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();

    ctf->SetColorSpaceToLab();
    ctf->SetScaleToLinear();
#if 0
    ctf->SetNanColor(0, 0, 0);// black
    ctf->SetAboveRangeColor(0, 0, 0);// black
    ctf->UseAboveRangeColorOn();
    ctf->SetBelowRangeColor(0, 0, 0);// black
    ctf->UseBelowRangeColorOn();
#endif
    ctf->AddRGBPoint(0, 0.05639999999999999, 0.05639999999999999, 0.47);
    ctf->AddRGBPoint(0.17159223942480895, 0.24300000000000013, 0.4603500000000004,
                     0.81);
    ctf->AddRGBPoint(0.2984914818394138, 0.3568143826543521, 0.7450246485363142,
                     0.954367702893722);
    ctf->AddRGBPoint(0.4321287371255907, 0.6882, 0.93, 0.9179099999999999);
    ctf->AddRGBPoint(0.5, 0.8994959551205902, 0.944646394975174,
                     0.7686567142818399);
    ctf->AddRGBPoint(0.5882260353170073, 0.957107977357604, 0.8338185108985666,
                     0.5089156299842102);
    ctf->AddRGBPoint(0.7061412605695164, 0.9275207599610714, 0.6214389091739178,
                     0.31535705838676426);
    ctf->AddRGBPoint(0.8476395308725272, 0.8, 0.3520000000000001,
                     0.15999999999999998);
    ctf->AddRGBPoint(1, 0.59, 0.07670000000000013, 0.11947499999999994);

    ctf->SetNumberOfValues(9);
    ctf->DiscretizeOn();

    return ctf;
}

}
