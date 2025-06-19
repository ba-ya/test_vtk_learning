#ifndef COMMON_H
#define COMMON_H

#include <QRegularExpression>
#include <QString.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>

static std::tuple<double, double, double> adj_rgb(QString hex_rgb) {
    static QRegularExpression reg("^#([\\da-fA-F]{2})([\\da-fA-F]{2})([\\da-fA-F]{2})");
    auto match = reg.match(hex_rgb);
    std::array<double, 3> rgb = {0.0, 0.0, 0.0};
    if (match.hasMatch()) {
        for (int i = 0; i < 3; i++) {
            bool ok = false;
            int value = match.captured(i + 1).toInt(&ok, 16);
            if (ok) {
                rgb[i] = value / 255.0;
            }
        }
    }
    return {rgb[0], rgb[1], rgb[2]};
}

static std::array<double, 4> get_viewport(int col, int row, int cols, int rows) {
    std::array<double, 4> viewport;
    // xmin, ymin, xmax, ymax
    viewport[0] = static_cast<double>(col) / cols;
    viewport[1] = static_cast<double>(rows - row - 1) / rows;
    viewport[2] = static_cast<double>(col + 1) / cols;
    viewport[3] = static_cast<double>(rows - row) / rows;
    return viewport;
}

static vtkSmartPointer<vtkActor> draw_point() {

    auto actor = vtkSmartPointer<vtkActor>::New();
    return actor;
}
#endif // COMMON_H
