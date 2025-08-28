# Tip

- [1, ExampleCells2d](#1-examplecells2d)
- [2, Cylinder3d](#2-cylinder3d)
- [3, SimpleOperations](#3-simpleoperations)
- [4, Axes3d](#4-axes3d)
- [5, ColorLine2d](#5-colorline2d)
- [6, Cube3d](#6-cube3d)
- [7, Dodecahedron3d](#7-dodecahedron3d)
- [8, EllipticalCylinder3d](#8-ellipticalcylinder3d)
- [9, GeometricObjectsDemo3d](#9-geometricobjectsdemo3d)
- [10, PlaneSourceDemo3d](#10-planesourcedemo3d)
- [11, Planes3d](#11-planes3d)
- [12, ShrinkCube3d](#12-shrinkcube3d)
- [13, SourceObjectsDemo3d](#13-sourceobjectsdemo3d)
- [14, Cell3DDemonstration3d](#14-cell3ddemonstration3d)
- [15, CellTypeSource3d](#15-celltypesource3d)
- [16, ConvexPointSet3d](#16-convexpointset3d)
- [17, Assembly3d](#17-assembly3d)
- [18, LinearCellsDemo3d](#18-linearcellsdemo3d)
- [19, ConesOnSphere3d](#19-conesonsphere3d)
- [20, EarthSource3d](#20-earthsource3d)
- [21, Frustum3d](#21-frustum3d)
- [22, OrientedArrow3d](#22-orientedarrow3d)
- [23, OrientedCylinder3d](#23-orientedcylinder3d)
- [24, Plane2d](#24-plane2d)
- [25, PlatonicSolids3d](#25-platonicsolids3d)

[26-50](./readme26-50.md)

Markdown All in One: Create Table of Contents, `vscode`生成`github`可读取的目录

# 1, ExampleCells2d

[回到顶部](#tip)

显示10*10的单元格

![image-20250612093016429](README.assets/image-20250612093016429.png)

# 2, Cylinder3d

[回到顶部](#tip)

 cylinder->SetResolution(6);

根据resolution绘制多个顶点

![image-20250612113155477](README.assets/image-20250612113155477.png)

# 3, SimpleOperations

[回到顶部](#tip)

// new version

![image-20250731114750575](README.assets/image-20250731114750575.png)

// old version

一些计算,没有图像,在输出查看,具体看代码

![image-20250616090333140](README.assets/image-20250616090333140.png)

![image-20250616090426010](README.assets/image-20250616090426010.png)

![image-20250616090507642](README.assets/image-20250616090507642.png)

![image-20250616090603652](README.assets/image-20250616090603652.png)

![image-20250616090619741](README.assets/image-20250616090619741.png)

![image-20250616090728487](README.assets/image-20250616090728487.png)

![image-20250616092646143](README.assets/image-20250616092646143.png)

# 4, Axes3d

[回到顶部](#tip)

![image-20250613133031469](README.assets/image-20250613133031469.png)

# 5, ColorLine2d

[回到顶部](#tip)

![image-20250613150125935](README.assets/image-20250613150125935.png)

# 6, Cube3d

[回到顶部](#tip)

把sample op移到第一个了,免得不小心点到

两种形式生成cube,一种自己给点给面(polydata),另一种是直接用自带的source(vtkCubeSource)

![image-20250616092853225](README.assets/image-20250616092853225.png)

# 7, Dodecahedron3d

[回到顶部](#tip)

![image-20250616130742374](README.assets/image-20250616130742374.png)

# 8, EllipticalCylinder3d

[回到顶部](#tip)

![image-20250617160516692](README.assets/image-20250617160516692.png)

添加了箭头

![image-20250618100811640](README.assets/image-20250618100811640.png)

# 9, GeometricObjectsDemo3d

[回到顶部](#tip)

![image-20250619131925877](README.assets/image-20250619131925877.png)

# 10, PlaneSourceDemo3d

[回到顶部](#tip)

![image-20250619161838252](README.assets/image-20250619161838252.png)

# 11, Planes3d

[回到顶部](#tip)

![image-20250620170517918](README.assets/image-20250620170517918.png)

# 12, ShrinkCube3d

[回到顶部](#tip)

![image-20250627090213754](README.assets/image-20250627090213754.png)

# 13, SourceObjectsDemo3d

[回到顶部](#tip)

![image-20250627100644584](README.assets/image-20250627100644584.png)

# 14, Cell3DDemonstration3d

[回到顶部](#tip)

[learn](./examples/readme_unstructuredGrid.md)

![image-20250715101929611](README.assets/image-20250715101929611.png)

# 15, CellTypeSource3d

[回到顶部](#tip)

![image-20250722130327287](README.assets/image-20250722130327287.png)

# 16, ConvexPointSet3d

[回到顶部](#tip)

[learn](./examples/readme_16vtkGlyph3DMapper.md)

![image-20250724110056715](README.assets/image-20250724110056715.png)

# 17, Assembly3d

[回到顶部](#tip)

![image-20250730094053910](README.assets/image-20250730094053910.png)

# 18, LinearCellsDemo3d

[回到顶部](#tip)

![image-20250731102925814](README.assets/image-20250731102925814.png)

# 19, ConesOnSphere3d

[回到顶部](#tip)

![image-20250731135700723](README.assets/image-20250731135700723.png)

# 20, EarthSource3d

[回到顶部](#tip)

![image-20250808150834903](README.assets/image-20250808150834903.png)

# 21, Frustum3d

[回到顶部](#tip)

![image-20250808161844268](README.assets/image-20250808161844268.png)

# 22, OrientedArrow3d

[回到顶部](#tip)

```c++
#if 0
    // no new data created
    mapper->SetInputConnection(arrow_source->GetOutputPort());
    actor->SetUserTransform(transfrom);
#else
    // create new polydata
    auto transform_pd = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transform_pd->SetTransform(transfrom);
    transform_pd->SetInputConnection(arrow_source->GetOutputPort());
    mapper->SetInputConnection(transform_pd->GetOutputPort());
#endif
```

![image-20250811085628878](README.assets/image-20250811085628878.png)

# 23, OrientedCylinder3d

[回到顶部](#tip)

```c++
// 只有heigth方向(自身坐标系y方向)scale 长度到length 
vtkMath::Subtract(end_p, start_p, normal_y);
// 平移到中点
matrix->SetElement(0, 3, (start_p[0] + end_p[0]) / 2);
```

![image-20250811110243955](README.assets/image-20250811110243955.png)

# 24, Plane2d

[回到顶部](#tip)

![image-20250811142400559](README.assets/image-20250811142400559.png)

# 25, PlatonicSolids3d

[回到顶部](#tip)

![image-20250813153308373](README.assets/image-20250813153308373.png)
