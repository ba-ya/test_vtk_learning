#ifndef COMMON_H
#define COMMON_H

#include <QRegularExpression>
#include <QString.h>
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

#endif // COMMON_H
