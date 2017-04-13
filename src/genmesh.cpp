/** Program used to generate meshes
 */

#include <fstream>
#include "utils.h"
#include "const.h"
#include "surface.h"

int main()
{
    auto surfaces = Surface::load(INPUT_FILE);
    std::ofstream os(MESH_FILE);
    float interval(1.0 / MESH_1D_NUM);
    for (const std::unique_ptr<Surface> &surf : surfaces)
    {
        for (int i = 0; i < MESH_1D_NUM + ! surf->uIsCircular(); i++)
            for (int j = 0; j < MESH_1D_NUM + ! surf->vIsCircular(); j++)
                os << "v " << surf->position(i * interval, j * interval) << std::endl;
        for (int i = 0; i < MESH_1D_NUM; i++)
            for (int j = 0; j < MESH_1D_NUM; j++)
            {
                int _i(i < MESH_1D_NUM - 1 || ! surf->uIsCircular() ? i + 1 : 0);
                int _j(j < MESH_1D_NUM - 1 || ! surf->vIsCircular() ? j + 1 : 0);
                int p0(i * MESH_1D_NUM + j + 1);
                int p1(i * MESH_1D_NUM + _j + 1);
                int p2(_i * MESH_1D_NUM + _j + 1);
                int p3(_i * MESH_1D_NUM + j + 1);
                os << "f " << p0 << " " << p1 << " " << p2 << " " << p3 << std::endl;
            }
    }
    return 0;
}

