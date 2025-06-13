#ifndef _1_CELLS_H
#define _1_CELLS_H

#include "common.h"
#include <vtkRenderer.h>

namespace SimpleOperations {
void Draw(vtkRenderer* renderer);
}

namespace ExampleCells2d {
void Draw(vtkRenderer* renderer);
}

namespace Cylinder3d {
void Draw(vtkRenderer* renderer);
}

namespace Axes3d {
void Draw(vtkRenderer* renderer);
}

namespace ColorLine2d {
void Draw(vtkRenderer* renderer);
}

namespace Cube3d {
void Draw(vtkRenderer* renderer);
}

#endif // _1_2D_CELLS_H
