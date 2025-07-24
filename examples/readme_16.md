### 🟡 第一种方法：使用 `vtkGlyph3DMapper`

```cpp
auto polydata = vtkSmartPointer<vtkPolyData>::New();
polydata->SetPoints(points);

auto point_mapper = vtkSmartPointer<vtkGlyph3DMapper>::New();
point_mapper->SetInputData(polydata);  // 设置点数据
point_mapper->SetSourceConnection(sphere->GetOutputPort());  // 设置球体作为“符号”

auto point_actor = vtkSmartPointer<vtkActor>::New();
point_actor->SetMapper(point_mapper);
point_actor->GetProperty()->SetColor(colors->GetColor3d("Peacock").GetData());

renderer->AddActor(point_actor);
```

### ✅ 解释：

1. **创建 `vtkPolyData` 并设置点**：

   * `vtkPoints` 中的每个点通过 `InsertNextPoint(...)` 插入，表示一个坐标。
   * 这些点会被传递给 `vtkPolyData`，这个类表示的是一个包含点、线、面等元素的几何对象。

2. **使用 `vtkGlyph3DMapper`**：

   * `vtkGlyph3DMapper` 是一个非常强大的类，用来**为每个点生成“符号”**，比如可以使用球体、箭头、立方体等几何体。
   * 在这里，`SetSourceConnection(sphere->GetOutputPort())` 表示每个点都会被替换为一个球体（因为 `sphere` 是 `vtkSphereSource` 创建的球体）。
   * 这样，点就不再是简单的坐标，而是**变成了一个小球**，其位置依赖于 `points` 中的每个点坐标。

3. **设置 Actor 和渲染**：

   * `vtkActor` 是渲染图形的基本单位，负责接收数据并将它渲染到窗口中。
   * `point_mapper` 被设置为 `vtkGlyph3DMapper`，因此每个点的位置会显示为一个小球。
   * 最后，将 `point_actor` 加入到渲染器中进行渲染。

---

### ✅ **为什么第一种方法常用？**

#### **优点：**

* **简洁、高效**：通过 `vtkGlyph3DMapper`，你不需要为每个点手动创建一个单独的 `vtkActor`。它会根据每个点的坐标自动生成一个球体，并统一管理显示。
* **灵活性高**：你可以更容易地改变点的表现方式（例如把点显示为球、箭头或立方体等）。`vtkGlyph3DMapper` 提供了更强的灵活性和扩展性。
* **减少冗余代码**：不需要显式创建每个点的 `vtkActor`，而是由 `vtkGlyph3DMapper` 统一管理，这减少了代码的重复性和复杂度。

#### **适用场景：**

* 如果你有大量的点，并且每个点都需要用相同的几何体（例如球）进行可视化，**第一种方法更简洁高效**。
* 当点的数量非常大时（例如成千上万），`vtkGlyph3DMapper` 比逐个创建 `vtkActor` 性能要高得多，因为它只创建一个 `mapper` 并为每个点渲染一个相同的几何体。

---

### 🟢 第二种方法：为每个点创建 `vtkActor`

```cpp
for (int i = 0; i < 13; i++) {
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphere->GetOutputPort());  // 每个点一个球体

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetPosition(points->GetPoint(i));  // 为每个球体设置位置
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("Peacock").GetData());
    renderer->AddActor(actor);
}
```

### ✅ 解释：

1. **每个点创建一个 `vtkActor`**：

   * 这种方法为每个点单独创建一个 `vtkActor`，并设置它的位置。
   * `SetPosition(points->GetPoint(i))` 用于将每个球体放置到指定的点位置上。

2. **优点和缺点**：

   * **优点**：可以单独设置每个球体的属性，例如颜色、大小、材质等，灵活性较高。
   * **缺点**：当点的数量非常多时，这种方法会导致性能问题，因为每个点都要创建一个 `vtkActor`，渲染时需要为每个球体分别处理。

### ✅ 适用场景：

* 当你需要对每个球体进行**单独控制**（例如不同颜色、大小、透明度等），或者想对每个点执行不同的操作时，使用第二种方法。
* **点数量较少时**，这种方法更加直观，代码也更易理解。

---

## ✅ 总结：两者的对比

| 特性        | **第一种方法（`vtkGlyph3DMapper`）** | **第二种方法（`vtkActor`）**          |
| --------- | ----------------------------- | ------------------------------ |
| **点渲染方式** | 统一显示为球体（或其他几何体）               | 每个点都有自己的 `vtkActor` 和 `mapper` |
| **渲染性能**  | 高效，适用于大量点                     | 性能较差，适合点数较少的情况                 |
| **灵活性**   | 灵活性较低，只能设置符号（球、立方体等）          | 可以为每个点单独设置不同的属性（大小、颜色等）        |
| **适用场景**  | 大量点、相同显示方式                    | 少量点、需要单独设置属性时                  |

---

### 结论：

* **第一种方法（`vtkGlyph3DMapper`）** 适合大量点、需要统一显示方式的场景，性能更好，代码更简洁。
* **第二种方法（逐点创建 `vtkActor`）** 更适合需要对每个点单独控制的场景，尤其当点数较少时。

## `vtkGlyph3DMapper` 

`vtkGlyph3DMapper` 是一个非常强大的类，用于将数据点显示为统一的符号（如小球、箭头等），每个点的位置由 `vtkPoints` 提供，符号的类型由你选择的几何体（如 `vtkSphereSource`）决定。

**Glyph** 是“符号”的意思，表示一个简单的几何体，用来表示一个数据点。

通过 `vtkGlyph3DMapper`，你可以非常轻松地将一个点集显示为各种符号，进行灵活的数据可视化。