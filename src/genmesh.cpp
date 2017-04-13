/** Program used to generate meshes
 */

#include <fstream>
#include "mesh.h"
#include "const.h"

int main()
{
    std::ofstream os(MESH_FILE);
    Mesh::outputSurfaces(os);
    return 0;
}

