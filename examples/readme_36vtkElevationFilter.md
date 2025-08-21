- [**1. vtkElevationFilter 相关函数**](#1-vtkelevationfilter-相关函数)
    - [`SetLowPoint(double x, double y, double z)`](#setlowpointdouble-x-double-y-double-z)
    - [`SetHighPoint(double x, double y, double z)`](#sethighpointdouble-x-double-y-double-z)
    - [`SetInputConnection(vtkAlgorithmOutput* input)`](#setinputconnectionvtkalgorithmoutput-input)
- [**2. vtkPolyDataMapper 相关函数**](#2-vtkpolydatamapper-相关函数)
    - [`SetLookupTable(vtkScalarsToColors* lut)`](#setlookuptablevtkscalarstocolors-lut)
    - [`SetColorModeToMapScalars()`](#setcolormodetomapscalars)
    - [`InterpolateScalarsBeforeMappingOn()`](#interpolatescalarsbeforemappingon)
  - [**整体流程**](#整体流程)
  - [**如果不用 ElevationFilter 会怎样？**](#如果不用-elevationfilter-会怎样)
- [vtkDiscretizableColorTransferFunction](#vtkdiscretizablecolortransferfunction)
    - [**1. `SetColorSpaceToLab()`**](#1-setcolorspacetolab)
    - [**2. `SetScaleToLinear()`**](#2-setscaletolinear)
    - [**3. `SetNanColor(double r, double g, double b)`**](#3-setnancolordouble-r-double-g-double-b)
    - [**4. `SetAboveRangeColor(double r, double g, double b)` + `UseAboveRangeColorOn()`**](#4-setaboverangecolordouble-r-double-g-double-b--useaboverangecoloron)
    - [**5. `SetBelowRangeColor(double r, double g, double b)` + `UseBelowRangeColorOn()`**](#5-setbelowrangecolordouble-r-double-g-double-b--usebelowrangecoloron)
    - [**总结：这段代码的整体含义**](#总结这段代码的整体含义)
    - [**如果不设置这些函数，会怎样？**](#如果不设置这些函数会怎样)
------

# **1. vtkElevationFilter 相关函数**

`vtkElevationFilter` 的主要作用：

> **根据点在某一方向上的高度（投影）生成标量值**，然后这些标量值就可以用于颜色映射。

### `SetLowPoint(double x, double y, double z)`

- 定义标量范围的起点（低端）。
- 任何接近这个点的几何位置，其生成的标量值会接近 **最小值**。

### `SetHighPoint(double x, double y, double z)`

- 定义标量范围的终点（高端）。
- 任何接近这个点的几何位置，其生成的标量值会接近 **最大值**。

### `SetInputConnection(vtkAlgorithmOutput* input)`

- 设置输入数据，一般接管其他 VTK 算子的输出。
- 这里是 **`cone->GetOutputPort()`**，即圆锥的几何数据。

**工作原理**：

- `vtkElevationFilter` 会根据几何中每个点沿着 **LowPoint → HighPoint 的方向**投影计算一个归一化值（0 ~ 1），并乘以标量范围（默认等于距离）。
- 这个标量值会附加到 **`PointData` 的 Scalars** 数组中，用于后续颜色映射。

------

# **2. vtkPolyDataMapper 相关函数**

`vtkPolyDataMapper` 是负责将几何数据映射成颜色和图形的核心类。你用到的几个函数都是跟 **颜色映射（scalar mapping）** 相关的。

### `SetLookupTable(vtkScalarsToColors* lut)`

- 设置查找表（LUT，Lookup Table），用来把标量值映射为颜色。
- 这里你用的是 `ctf = GetCTF()`（可能是 `vtkDiscretizableColorTransferFunction` 或其他色标）。

### `SetColorModeToMapScalars()`

- 告诉 mapper：使用标量值去查表映射颜色，而不是直接把标量值当颜色。
- 默认行为可能会把 [0,1] 标量直接当作 RGB，用这个可以避免。

### `InterpolateScalarsBeforeMappingOn()`

- 如果你的几何是插值渲染（Gouraud shading、Phong shading），那么标量会先在顶点间插值，再进行颜色映射。
- 如果关掉它（默认是 Off），VTK 会先在顶点做颜色映射，再插值颜色。
- 开启后颜色更平滑，适合连续数据（如温度、压力、elevation）。

------

## **整体流程**

1. `vtkElevationFilter` → 生成一个标量场（高度相关）。
2. `vtkPolyDataMapper` → 根据标量值查色标（`LookupTable`）得到颜色。
3. `actor->SetMapper(mapper)` → 将映射好的数据添加到渲染器。

------

## **如果不用 ElevationFilter 会怎样？**

- 几何就没有标量值了（或者用默认标量）。
- 颜色映射会失败（如果 `mapper->SetColorModeToMapScalars()` 依赖标量的话）。

------

# vtkDiscretizableColorTransferFunction

### **1. `SetColorSpaceToLab()`**

- 设置颜色插值所用的颜色空间。
- `Lab` 空间是感知均匀的颜色空间（L*a*b*），颜色过渡会更平滑、更符合人眼感知。
- 其他常见选项包括：
  - `SetColorSpaceToRGB()`（默认，线性 RGB 插值）
  - `SetColorSpaceToHSV()`（色调饱和度明度空间）

------

### **2. `SetScaleToLinear()`**

- 定义标量值的缩放方式。
- 这里是 **线性缩放**，即标量值按线性关系映射到颜色。
- 其他可能选项：
  - `SetScaleToLog10()` → 适合对数分布数据（如声强、地震数据）。

------

### **3. `SetNanColor(double r, double g, double b)`**

- 设置 **标量值为 NaN（非数字）** 时的颜色。
- 这里设置为 **黑色 (0,0,0)**。

------

### **4. `SetAboveRangeColor(double r, double g, double b)` + `UseAboveRangeColorOn()`**

- 设置 **超出上限（大于最大标量范围）** 时使用的颜色。
- 这里设置为黑色，并且 **开启** 了使用该颜色（`UseAboveRangeColorOn()`）。
- 如果不调用 `UseAboveRangeColorOn()`，即使设置了颜色，也不会启用。

------

### **5. `SetBelowRangeColor(double r, double g, double b)` + `UseBelowRangeColorOn()`**

- 设置 **超出下限（小于最小标量范围）** 时使用的颜色。
- 也是黑色，并且 **开启** 了使用。

------

### **总结：这段代码的整体含义**

- 用 **Lab 空间** 进行颜色插值（平滑、感知均匀）。
- 标量值 **线性映射** 到颜色。
- 对于 **超出范围** 和 **NaN** 的数据，全部用 **黑色 (0,0,0)** 表示。

------

### **如果不设置这些函数，会怎样？**

- 默认用 RGB 空间插值，颜色可能出现不均匀过渡。
- NaN 值通常不会渲染（或者用白色/透明）。
- 超出范围的数据会被 **夹紧（Clamp）** 到边界值的颜色，而不会变成黑色。