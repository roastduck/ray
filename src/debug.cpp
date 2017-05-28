/** Test intersection manually
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
    std::vector<Ray> rays;
    for (float z = -10; z <= 5; z += 0.5)
        rays.push_back(Ray(Vec3(180, -180, 180), Vec3(-167.875, 200, z) - Vec3(180, -180, 180)));
    int cnt(0);
    for (const auto &ray : rays)
    {
        auto interOpt = surface->findInter(ray);
        if (! interOpt.isOk())
        {
            std::cout << cnt++ << ": No intersection" << std::endl;
            continue;
        }
        auto inter(interOpt.ok());
        std::cout << cnt++ << ": t = " << inter.t << std::endl;
    }
    return 0;
}

