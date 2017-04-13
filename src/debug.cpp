/** Just to output something to a .obj file
 */

#include <fstream>
#include "mesh.h"
#include "const.h"
#include "surface.h"

int main()
{
    std::ofstream os("debug.obj");
    Mesh::outputSurfaces(os);
    auto surfaces = Surface::load(INPUT_FILE);
    float interval(1.0 / 40);
    for (const std::unique_ptr<Surface> &surf : surfaces)
        for (int i = 0; i < 1; i++)
            for (int j = 0; j < 40; j++)
            {
                int _i(i < MESH_1D_NUM - 1 || ! surf->uIsCircular() ? i + 1 : 0);
                int _j(j < MESH_1D_NUM - 1 || ! surf->vIsCircular() ? j + 1 : 0);
                Mesh::outputBox3(os, surf->xyzMinMax(i * interval, _i * interval, j * interval, _j * interval));
            }
    return 0;
}

