`VelocityMagnitude` 并不是你随便写的名字，而是 **PLOT3D 数据读入时自动生成的标量/向量数组名字之一**。

------

## 🔹 PLOT3D Reader 的数据数组

当你调用

```cpp
pl3d->SetScalarFunctionNumber(100);
pl3d->SetVectorFunctionNumber(202);
pl3d->AddFunction(153);
```

实际上就是告诉 Reader 要计算哪些物理量。
 VTK 内部会根据 **NASA PLOT3D 数据格式** 定义的一些“函数编号”（Function Numbers），生成不同的数组。

 * The scalar functions are:
 *    **-1  - don't read or compute any scalars**
 *    **100 - density**
 *    **110 - pressure**
 *    **111 - pressure coefficient (requires Overflow file with Gamma)**
 *    **112 - mach number (requires Overflow file with Gamma)**
 *    **113 - sounds speed (requires Overflow file with Gamma)**
 *    **120 - temperature**
 *    **130 - enthalpy**
 *    **140 - internal energy**
 *    **144 - kinetic energy**
 *    **153 - velocity magnitude**
 *    **163 - stagnation energy**
 *    **170 - entropy**
 *    **184 - swirl**
 *    **211 - vorticity magnitude**

 * The vector functions are:
 *    **-1  - don't read or compute any vectors**
 *    **200 - velocity**
 *    **201 - vorticity**
 *    **202 - momentum**
 *    **210 - pressure gradient.**
 *    **212 - strain rate**

------

## 🔹 数据数组的存储位置

- 向量场（如 `"Velocity"`）通常存放在 **PointData / Vectors** 里。
- 标量场（如 `"Pressure"`, `"VelocityMagnitude"`）存放在 **PointData / Scalars** 里。

当你 `AddFunction(202)` 时，Reader 会计算出 **速度矢量的模长**，并在数据里新增一个数组 `"VelocityMagnitude"`。

所以你在 `ColorByArrayComponent("VelocityMagnitude", 0)` 时，实际上是在告诉 Mapper：
 👉 取 `"VelocityMagnitude"` 这个数组里的第 0 个分量（标量，所以只有一个分量），用来映射颜色。

------

## 🔹 如果你不用官方函数

如果你自己手工构造一个 `vtkStructuredGrid`，没有调用 PLOT3D Reader，那么默认是 **没有 `VelocityMagnitude` 这个数组的**。
 这时你就需要自己用 `vtkDoubleArray` 添加标量：

```cpp
auto scalars = vtkSmartPointer<vtkDoubleArray>::New();
scalars->SetName("MyScalars");
scalars->SetNumberOfComponents(1);
scalars->SetNumberOfTuples(numPoints);

for (int i = 0; i < numPoints; ++i) {
    scalars->SetValue(i, some_value);
}

sgrid->GetPointData()->SetScalars(scalars);
```

之后你就可以在 Mapper 里写 `ColorByArrayComponent("MyScalars", 0)` 来着色。

------

✅ 总结：

- `VelocityMagnitude` 并不是 C++ 关键字，而是 **VTK PLOT3D Reader 自动生成的数组名字**。
- 这个名字来自函数编号（202），VTK 内部有一张映射表。
- 如果你不用 PLOT3D Reader，就需要自己创建数组并命名。
