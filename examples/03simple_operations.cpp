#include "00headers.h"
#include <vtkMath.h>
#include <vtkQuad.h>
#include <vtkLine.h>
#include <vtkBoxMuellerRandomSequence.h>
#include <vtkMatrix4x4.h>
#include <vtkPerspectiveTransform.h>
#include <vtkTransform.h>
#include <vtkPlane.h>
#include <vtkMinimalStandardRandomSequence.h>
#include "QDebug.h"

void SimpleOperations::Draw(vtkRenderer *renderer)
{
    qDebug() << "";
    qDebug() << "-----------SimpleOperations-----------";
    {
        qDebug() << u8"---计算两个点的距离";
        double p0[3] = {0.0, 0.0, 0.0};
        double p1[3] = {1.0, 1.0, 1.0};

        double dis_squared = vtkMath::Distance2BetweenPoints(p0, p1);
        double dis = std::sqrt(dis_squared);
        qDebug() << "dis_squared:" << dis_squared << ", dis:" << dis;
    }

    qDebug() << u8"---计算点到线的距离";
    {
        double line_p0[3] = {0.0, 0.0, 0.0};
        double line_p1[3] = {0.0, 6.0, 0.0};

        double p0[3] = {5, 4.0, 0.0};
        // 垂直距离
        auto dis_0_squared = vtkLine::DistanceToLine(p0, line_p0, line_p1);
        auto dis_0 = std::sqrt(dis_0_squared);
        qDebug() << "dis_0_squared" << dis_0_squared << ", dis_0" << dis_0;
        // 垂直距离, 并求出 点 p0 到线段 line 所做垂线的垂足（垂点）的位置和坐标。
        // 如果 t == 0：最近点就是 line_p0
        // 如果 t == 1：最近点是 line_p1
        // 如果 0 < t < 1：最近点在线段内部
        // 如果 t < 0 或 t > 1：最近点在线段延长线上（注意：这个时候 closest 可能在段外）
        double t;
        double closest[3];
        double dis_1_squared = vtkLine::DistanceToLine(p0, line_p0, line_p1, t, closest);
        auto dis_1 = std::sqrt(dis_1_squared);
        qDebug() << "dis_1_squared" << dis_1_squared<< ", dis_1" << dis_1
                 << ", t" << t
                 << ", closest point" << closest[0] << closest[1] << closest[2];
    }

    qDebug() << u8"---正态分布得到随机数";
    {
        auto random_source = vtkSmartPointer<vtkBoxMuellerRandomSequence>::New();
        // mean	正态分布的期望值（期望/均值），决定分布的中心, 确定正态分布曲线的中点是(mean, 0)
        // standard_deviation 标准差，决定“扩散程度” —— 越大越分散, 确定正态分布曲线的左边点和右边点[-标准差^2, +标准差^2]
        // GetScaledValue(mean, sd)	生成一个符合 N(mean, sd²) 的随机值
        auto mean = 0.0;
        auto standard_deviation = 2.0;
        qDebug() << "mean" << mean << ", standard_deviation" << standard_deviation;
        for (int i = 0; i < 3; ++i) {
            auto a = random_source->GetScaledValue(mean, standard_deviation);
            random_source->Next();
            qDebug() << i << a;
        }
    }

    qDebug() << u8"---透视变换";
    {
        // 一般是 vtkTransform进行基础变换, 再用 camere做出投影效果
        // 实际使用的时候,这个矩阵可以根据transform得到
        // auto transform = vtkSmartPointer<vtkTransform>::New();
        // transform->Translate(10, 0, 0); // 平移
        // transform->RotateZ(45);         // 旋转
        // transform->Scale(2, 2, 2);      // 缩放
        // auto m = transform->GetMatrix();
        auto m = vtkSmartPointer<vtkMatrix4x4>::New();
        int cnt_col = 4;
        int cnt_row = 4;
        std::vector<double> values = {
            1, 2, 3, 4,
            2, 2, 3, 4,
            3, 2, 3, 4,
            4, 2, 3, 4,};
        for (int i = 0; i < cnt_row; ++i) {
            for (int j = 0; j < cnt_col; ++j) {
                m->SetElement(i, j, values[i * cnt_row +j]);
            }
        }

        double p[3] = {1.0, 2.0, 3.0};

        // |1, 2, 3, 4|   |1|   |18|
        // |2, 2, 3, 4|   |2|   |19|
        // |3, 2, 3, 4| * |3| = |20|
        // |4, 2, 3, 4|   |1|   |21|
        //             拓展为齐次坐标

        // 结果是18 19 20
        auto transform = vtkSmartPointer<vtkTransform>::New();
        transform->SetMatrix(m);
        double normal_projection[3];
        transform->TransformPoint(p, normal_projection);
        qDebug() << "standard projection:"
                 << normal_projection[0]
                 << normal_projection[1]
                 << normal_projection[2];

        // 结果是 18/21 19/21 20/21
        auto perspective_transform = vtkSmartPointer<vtkPerspectiveTransform>::New();
        perspective_transform->SetMatrix(m);
        double perspective_projection[3];
        perspective_transform->TransformPoint(p, perspective_projection);
        qDebug() << "perspective_projection:"
                 << perspective_projection[0]
                 << perspective_projection[1]
                 << perspective_projection[2];
    }

    qDebug() << u8"---点在面上的投影";
    {
        // 平面内一点和法线确定一个平面
        // 判断点在不在平面上
        // plane->EvaluateFunction(point)
        auto plane = vtkSmartPointer<vtkPlane>::New();
        double origin[3] = {0.0, 0.0, 0.0};
        double normal[3] = {0.0, 0.0, 1.0};// 法线一般归一化
        plane->SetOrigin(origin[0], origin[1], origin[2]);
        plane->SetNormal(normal[0], normal[1], normal[2]);

        // pp'与法向量平行,
        // t = op在normal上的投影长度,即op点乘normal, 23.1*0 + 54.6*0 + 9.2*1 = 9.2
        // pp' = t(0,0,1) = 9.2(0,0,1) = (0,0,9.2)
        // 又有 pp' = (23.1 - x', 54.6 - y', 9.2 - z')
        // (x', y', z') = (23.1 - 0, 54.6 - 0, 9.2 - 9.2)
        // 即 p' = p - (op点乘normal)normal
        double p[3] = {23.1, 54.6, 9.2};
        double projected[3];
        plane->ProjectPoint(p, origin, normal, projected);

        qDebug() << "project:"
                 << projected[0]
                 << projected[1]
                 << projected[2];
    }

    qDebug() << u8"---MinimalStandard得到随机数";
    {
        auto sequence = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
        // 用一seed,得到的随机数都是一样
        // 可以换成 sequence->SetSeed(time(NULL));
        sequence->SetSeed(1);

        for (int i = 0; i < 3; ++i) {
            auto x = sequence->GetValue();
            auto y = sequence->GetRangeValue(0, 2);
            sequence->Next();
            qDebug() << i << x << ", [0, 2]"<< y;
        }
    }

}
