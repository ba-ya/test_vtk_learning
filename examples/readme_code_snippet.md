[根据屏幕坐标得到射线](#根据屏幕坐标得到射线)

## 根据屏幕坐标得到射线

```c++
// 自定义一个结构体，表示 3D 射线：起点和方向
struct Ray3D {
    gp_Pnt origin;     // 射线起点（世界坐标）
    gp_Vec direction;  // 射线方向（单位向量）
};

// 将屏幕上的点 (x, y) 转换成一条射线，基于指定的 renderer（视图）
Ray3D screen_to_ray(double x, double y, vtkRenderer* renderer) {
    // 1. 获取当前激活的摄像机（用于获取观察位置）
    vtkCamera* camera = renderer->GetActiveCamera();

    // 2. 获取摄像机的位置作为射线的起点（世界坐标系中）
    double origin[3]; 
    camera->GetPosition(origin);

    // 3. 设置屏幕坐标（x, y）对应的显示点，z=0 代表近平面
    renderer->SetDisplayPoint(x, y, 0.0);

    // 4. 将屏幕坐标转换为世界坐标（内部会用投影和模型矩阵反变换）
    renderer->DisplayToWorld();

    // 5. 获取转换后的世界坐标点（这是 z=0 近平面上的一点）
    double near_pnt[4]; // 齐次坐标，最后一维通常为1
    renderer->GetWorldPoint(near_pnt);

    // 6. 返回 Ray3D 结构体，包含起点和方向向量（单位化）
    return {
        gp_Pnt(origin[0], origin[1], origin[2]), // 原点：摄像机位置
        gp_Vec(
            near_pnt[0] - origin[0],             // x方向分量
            near_pnt[1] - origin[1],             // y方向分量
            near_pnt[2] - origin[2]              // z方向分量
        ).Normalized()                           // 单位向量化，得到方向
    };
}

```

