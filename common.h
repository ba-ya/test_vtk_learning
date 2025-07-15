#ifndef COMMON_H
#define COMMON_H

#include <QRegularExpression>
#include <QString.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

namespace Helper {
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

static std::array<double, 4> get_viewport(int row, int col, int rows, int cols) {
    std::array<double, 4> viewport;
    // xmin, ymin, xmax, ymax
    viewport[0] = static_cast<double>(col) / cols;
    viewport[1] = static_cast<double>(rows - row - 1) / rows;
    viewport[2] = static_cast<double>(col + 1) / cols;
    viewport[3] = static_cast<double>(rows - row) / rows;
    return viewport;
}

static void layout_renders_in_grid(std::vector<vtkSmartPointer<vtkRenderer>> renders,
                                   std::vector<vtkSmartPointer<vtkActor>> actors,
                                   std::vector<vtkSmartPointer<vtkActor2D>> text_actors,
                                   int grid_rows, int grid_cols) {
    for (int row = 0; row < grid_rows; ++row) {
        for (int col = 0; col < grid_cols; ++col) {
            auto index = row * grid_cols + col;
            auto viewport = get_viewport(row, col, grid_rows, grid_cols);
            if (index >= renders.size()) {
                break;
            }
            renders[index]->SetViewport(viewport.data());
            if (index < actors.size()) {
                renders[index]->AddActor(actors[index]);
            }
            if (index < text_actors.size()) {
                renders[index]->AddActor2D(text_actors[index]);
            }

        }
    }
}

}

#endif // COMMON_H
