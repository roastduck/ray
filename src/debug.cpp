/** Just to output something to a .obj file
 */

#include <fstream>
#include "mesh.h"
#include "const.h"
#include "surface.h"

int main()
{
    auto surfaces = Surface::load(INPUT_OBJECTS_FILE);
    const auto &surface = surfaces[0];
    assert(surface);
    std::vector<Ray> rays = {
        Ray(Vec3(0, -10, 0), Vec3(0.05, 1, 0.5))
    };
    for (const auto &ray : rays)
    {
        auto interOpt = surface->findInter(ray);
        if (! interOpt.isOk())
        {
            std::cout << "No intersection" << std::endl;
            continue;
        }
        auto inter(interOpt.ok());
        std::cout << "t = " << inter.t << std::endl;
    }
    return 0;
}

