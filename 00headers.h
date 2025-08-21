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

namespace Dodecahedron3d {
void Draw(vtkRenderer* renderer);
}

namespace EllipticalCylinder3d {
void Draw(vtkRenderer* renderer);
}

namespace GeometricObjectsDemo3d {
void Draw(std::vector<vtkSmartPointer<vtkRenderer>> renders);
}

namespace PlaneSourceDemo3d {
void Draw(vtkRenderer* renderer);
}

namespace Planes3d {
void Draw(std::vector<vtkSmartPointer<vtkRenderer>> renders);
}

namespace ShrinkCube3d {
void Draw(vtkRenderer* renderer);
}

namespace SourceObjectsDemo3d {
void Draw(std::vector<vtkSmartPointer<vtkRenderer>> renders);
}

namespace Cell3DDemonstration3d {
void Draw(std::vector<vtkSmartPointer<vtkRenderer>> renders);
}

namespace CellTypeSource3d {
void Draw(vtkRenderer* renderer);
}

namespace ConvexPointSet3d {
void Draw(vtkRenderer* renderer);
}

namespace Assembly3d {
void Draw(vtkRenderer* renderer);
}

namespace LinearCellsDemo3d {
void Draw(std::vector<vtkSmartPointer<vtkRenderer>> renders);
}

namespace ConesOnSphere3d {
void Draw(vtkRenderer* renderer);
}

namespace EarthSource3d {
void Draw(vtkRenderer* renderer);
}

namespace Frustum3d {
void Draw(vtkRenderer* renderer);
}

namespace OrientedArrow3d {
void Draw(vtkRenderer* renderer);
}

namespace OrientedCylinder3d {
void Draw(vtkRenderer* renderer);
}

namespace Plane2d {
void Draw(vtkRenderer* renderer);
}

namespace PlatonicSolids3d {
void Draw(std::vector<vtkSmartPointer<vtkRenderer>> renders);
}

namespace RegularPolygonSource2d {
void Draw(vtkRenderer* renderer);
}

namespace TessellatedBoxSource3d {
void Draw(vtkRenderer* renderer);
}

namespace IsoparametricCellsDemo3d {
void Draw(std::vector<vtkSmartPointer<vtkRenderer>> renders);
}

namespace LegendScaleActor2d {
void Draw(vtkRenderer* renderer);
}

namespace PolarAxesActor2d {
void Draw(vtkRenderer* renderer);
}

namespace TextOrigin3d {
void Draw(vtkRenderer* renderer);
}

namespace XYPlot {
void Draw(std::vector<vtkSmartPointer<vtkRenderer>> renders);
}

namespace ProbeCombustor3d {
void Draw(vtkRenderer* renderer);
}

namespace ColorIsosurface3d {
void Draw(vtkRenderer* renderer);
}

namespace BrownianPoints3d {
void Draw(vtkRenderer* renderer);
}

namespace ColorMapToLUT3d {
void Draw(vtkRenderer* renderer);
}


#endif // _1_2D_CELLS_H
