# 旋转和平移

右手系:x,y,z表示轴向量, `.`表示点乘

`x = y.z` `y = z.x ` `z = x.y`

### ✅ **手动构造 4x4 旋转矩阵的思路（重点只看方向，不看缩放）**

你要构造一个局部坐标系 —— 即 X/Y/Z 轴三基向量：

#### 假设你用目标“up”方向来构造局部坐标系的 Z 轴：

```cpp
Z_axis = Normalize(up_direction)
```

然后你需要任选一个 **参考轴**（不平行于 Z），比如全局的 X 轴 `(1, 0, 0)` 来计算：

```cpp
X_axis = Normalize(Cross(global_reference, Z_axis))  // 与 Z 正交
Y_axis = Cross(Z_axis, X_axis)  // 保证右手系
```

然后构造一个 3x3 的旋转矩阵（列向量是 X, Y, Z）：

```
| Xx Yx Zx Tx |
| Xy Yy Zy Ty |
| Xz Yz Zz Tz |
|  0  0  0  1 |
```

其中 `(Tx, Ty, Tz)` 是平移部分，也就是 `pos`。

### 代码举例

传入目标位置,和up方向的向量(这里up方向表示的是y轴方向)

```c++
vtkSmartPointer<vtkMatrix4x4> ShapeActorHandler::create_transform_matrix(const gp_Pnt &pos, const gp_Vec &up_direction)
{
    gp_Vec y = up_direction.Normalized();
    gp_Vec x = y.Crossed(gp_Vec(0, 0, 1));
    if (x.Magnitude() < 1e-6)
        x = gp_Vec(1, 0, 0);
    x.Normalize();
    gp_Vec z = x.Crossed(y);
    z.Normalize();

    auto mat = vtkSmartPointer<vtkMatrix4x4>::New();
    mat->Identity();
    for (int i = 0; i < 3; ++i) {
        auto v =
                i == 0 ? 1 * x :
                i == 1 ? 1 * y : 1 * z;
        // x,y,z是单位向量,可以乘上缩放因子,就可以缩放了
        mat->SetElement(0, i, v.X());
        mat->SetElement(1, i, v.Y());
        mat->SetElement(2, i, v.Z());
        mat->SetElement(3, i, 0.0);
    }
    mat->SetElement(0, 3, pos.X());
    mat->SetElement(1, 3, pos.Y());
    mat->SetElement(2, 3, pos.Z());
    mat->SetElement(3, 3, 1.0);

    return mat;
}
```

