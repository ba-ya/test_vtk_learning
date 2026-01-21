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
    // 右手系, y轴方向作为up朝向
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
    // 最后一列是平移的坐标
    mat->SetElement(0, 3, pos.X());
    mat->SetElement(1, 3, pos.Y());
    mat->SetElement(2, 3, pos.Z());
    mat->SetElement(3, 3, 1.0);

    return mat;
}
```



## 解释两种设置矩阵的方式

```c++
    auto transform = vtkSmartPointer<vtkTransform>::New();
#if 0
    transform->SetMatrix(matrix);
    transform->PostMultiply(); // 之后的操作在世界坐标系下进行
    transform->Translate(start_point);
#else
    transform->Translate(start_point);
    transform->Concatenate(matrix);
#endif
```

## 一、先给结论（先看这个）

### 1️⃣ `SetMatrix`

> **直接把 transform 的内部矩阵“整体替换”成你给的矩阵**

- 之前所有变换全部清空

- 不参与乘法顺序

- 相当于：

  ```cpp
  transform = matrix;
  ```

### 2️⃣ `Concatenate`

> **把你给的矩阵“乘到当前 transform 的矩阵上”**

- 保留已有变换

- 会受到 `PreMultiply / PostMultiply` 影响

- 相当于：

  ```cpp
  transform = transform * matrix   // PostMultiply
  transform = matrix * transform   // PreMultiply
  ```

------

## 二、vtkTransform 内部到底在干什么？

`vtkTransform` 内部维护一个 **4×4 齐次矩阵 M**

任何操作本质都是：

```text
M = A × M   （PreMultiply）
M = M × A   （PostMultiply）
```

其中 `A` 是你这一步的变换（Translate / Rotate / Scale / Concatenate 等）

------

## 三、SetMatrix vs Concatenate（核心区别）

### ✅ `SetMatrix(matrix)`

```cpp
transform->SetMatrix(matrix);
```

等价于：

```text
M = matrix
```

特点：

| 特性   | 说明                   |
| ------ | ---------------------- |
| 覆盖   | 会清掉之前所有变换     |
| 无顺序 | 不受 Pre / Post 影响   |
| 常用于 | 已知一个“完整世界矩阵” |

> **你是在“告诉 VTK：这就是最终矩阵”**

------

### ✅ `Concatenate(matrix)`

```cpp
transform->Concatenate(matrix);
```

等价于：

```text
M = M × matrix    // PostMultiply（默认）
M = matrix × M    // PreMultiply
```

特点：

| 特性   | 说明                 |
| ------ | -------------------- |
| 累积   | 在已有变换基础上继续 |
| 有顺序 | 和 Pre / Post 强相关 |
| 常用于 | 逐步构建变换         |

------

## 四、PreMultiply / PostMultiply 是什么意思？

这是整个问题的 **灵魂部分**。

### 1️⃣ 默认是 PostMultiply

```cpp
vtkTransform 默认：PostMultiply
```

含义：

```text
M = M × A
```

**新的变换发生在“当前坐标系”下（局部坐标）**

------

### 2️⃣ PostMultiply（局部坐标系）

```cpp
transform->PostMultiply();
transform->Translate(...);
```

数学含义：

```text
M = M × T
```

直觉理解：

> “沿着物体自己当前的轴方向移动”

📌 非常适合：

- 物体自身变换
- 局部旋转 / 平移

------

### 3️⃣ PreMultiply（世界坐标系）

```cpp
transform->PreMultiply();
transform->Translate(...);
```

数学含义：

```text
M = T × M
```

直觉理解：

> “在世界坐标系中移动物体”

📌 非常适合：

- 把物体整体挪到某个世界位置
- 对已经成型的物体做整体操作

------

## 五、回到你给的两段代码（逐行解释）

### 🔹 第一种（`#if 0`）

```cpp
transform->SetMatrix(matrix);
transform->PostMultiply();
transform->Translate(start_point);
```

执行顺序：

1. `SetMatrix(matrix)`

   ```text
   M = matrix
   ```

2. `PostMultiply()`（设置乘法模式）

3. `Translate(start_point)`

   ```text
   M = matrix × T
   ```

📌 **平移发生在 matrix 定义的“局部坐标系”里**

------

### 🔹 第二种（`#else`）

```cpp
transform->Translate(start_point);
transform->Concatenate(matrix);
```

默认是 `PostMultiply`：

1. `Translate(start_point)`

   ```text
   M = I × T = T
   ```

2. `Concatenate(matrix)`

   ```text
   M = T × matrix
   ```

📌 **matrix 发生在平移之后，坐标语义完全不同**

------

## 六、为什么这两种写法“看起来差不多但结果不一样”？

因为：

```text
matrix × T  ≠  T × matrix
```

矩阵乘法 **不可交换**。

尤其当 matrix 里包含：

- 旋转
- 非等比缩放

差异会非常明显。

------

## 七、什么时候该用哪种？

### ✅ 用 `SetMatrix` 的典型场景

- 从外部系统导入世界矩阵（CAD / 相机 / 传感器）
- 已经算好一个完整 4×4 变换
- 不想让 VTK 再“帮你算顺序”

```cpp
transform->SetMatrix(worldMatrix);
```

------

### ✅ 用 `Concatenate + Pre/Post` 的场景

- 程序内逐步构建变换
- 清楚“这是世界坐标还是局部坐标”
- 做动画 / 交互

```cpp
transform->PostMultiply();  // 局部
transform->RotateZ(...);
transform->Translate(...);
```

或

```cpp
transform->PreMultiply();   // 世界
transform->Translate(...);
```

------

## 八、一个一句话记忆法（非常重要）

> **SetMatrix = 直接替换**
> **Concatenate = 参与乘法**
> **PostMultiply = 局部坐标**
> **PreMultiply = 世界坐标**