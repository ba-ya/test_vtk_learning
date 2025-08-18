# Tip

- [26, RegularPolygonSource2d](#26-regularpolygonsource2d)
- [27, TessellatedBoxSource3d](#27-tessellatedboxsource3d)
- [28, IsoparametricCellsDemo3d](#28-IsoparametricCellsDemo3d)
- [29, LegendScaleActor2d](#29-LegendScaleActor2d)
- [30, PolarAxesActor2d](#30-PolarAxesActor2d)
- [31, TextOrigin3d](#31-TextOrigin3d)
# 26, RegularPolygonSource2d

[回到顶部](#tip)

![image-20250813165153337](readme26-50.assets/image-20250813165153337.png)

# 27, TessellatedBoxSource3d

[回到顶部](#tip)

![image-20250814110551393](readme26-50.assets/image-20250814110551393.png)

# 28, IsoparametricCellsDemo3d

[回到顶部](#tip)

[learn](./examples/readme_vtkNonLinearCell.md)

![image-20250815144724162](readme26-50.assets/image-20250815144724162.png)

# 29, LegendScaleActor2d

[回到顶部](#tip)

![image-20250815160655517](readme26-50.assets/image-20250815160655517.png)

# 30, PolarAxesActor2d

[回到顶部](#tip)

```c++
  auto polar_actor = vtkSmartPointer<vtkPolarAxesActor>::New();
  polar_actor->SetCamera(renderer->GetActiveCamera());
```



![image-20250818090933893](readme26-50.assets/image-20250818090933893.png)

# 31, TextOrigin3d

[回到顶部](#tip)

```c++
auto text_actor = vtkSmartPointer<vtkFollower>::New();
text_actor->SetCamera(renderer->GetActiveCamera());
```

![image-20250818093000430](readme26-50.assets/image-20250818093000430.png)