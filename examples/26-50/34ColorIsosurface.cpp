#include "00headers.h"

#include <vtkContourFilter.h>
#include <vtkLODActor.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkMultiBlockPLOT3DReader.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>
#include <vtkStructuredGrid.h>
#include <vtkStructuredGridOutlineFilter.h>

void ColorIsosurface3d::Draw(vtkRenderer *renderer)
{
    auto colors = vtkSmartPointer<vtkNamedColors>::New();

    auto pl3d = vtkSmartPointer<vtkMultiBlockPLOT3DReader>::New();
    pl3d->SetXYZFileName(Helper::path("combxyz.bin"));
    pl3d->SetQFileName(Helper::path("combq.bin"));
    pl3d->SetScalarFunctionNumber(100);
    pl3d->SetVectorFunctionNumber(202);
    pl3d->AddFunction(153);
    pl3d->Update();

    auto sg = dynamic_cast<vtkStructuredGrid *>(pl3d->GetOutput()->GetBlock(0));

    auto iso = vtkSmartPointer<vtkContourFilter>::New();
    iso->SetInputData(sg);
    iso->SetValue(0, .24);

    auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->SetInputConnection(iso->GetOutputPort());
    // 控制曲面是“光滑阴影”还是“硬边阴影”。
    // 如果两个相邻面之间的夹角 小于 45°，则它们的法线会被平滑平均 → 表现为一个连续光滑的曲面。
    normals->SetFeatureAngle(45);

    auto iso_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    iso_mapper->SetInputConnection(normals->GetOutputPort());
    iso_mapper->ScalarVisibilityOn();// 开启标量着色
    iso_mapper->SetScalarRange(0, 1500);// 定义颜色范围
    iso_mapper->SetScalarModeToUsePointFieldData(); // 使用点数据
    // 对应上面 pl3d->AddFunction(153);
    iso_mapper->ColorByArrayComponent("VelocityMagnitude", 0); // 根据速度大小来着色

    auto iso_actor = vtkSmartPointer<vtkLODActor>::New();
    iso_actor->SetMapper(iso_mapper);
    iso_actor->SetNumberOfCloudPoints(1000);

    // outline
    auto outline = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
    outline->SetInputData(sg);
    auto outline_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    outline_mapper->SetInputConnection(outline->GetOutputPort());
    auto outline_actor = vtkSmartPointer<vtkActor>::New();
    outline_actor->SetMapper(outline_mapper);
    outline_actor->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());
    outline_actor->GetProperty()->SetLineWidth(2);

    renderer->AddActor(iso_actor);
    renderer->AddActor(outline_actor);
}
