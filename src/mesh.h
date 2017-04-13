#ifndef MESH_H_
#define MESH_H_

#include "utils.h"
#include <iostream>

/** Class used to manage outputing to obj file
 *  This is a static class
 */
class Mesh
{
private:
    static int vertCnt;

public:
    /// Output all surfaces as meshes
    static void outputSurfaces(std::ostream &os);

    /// Output a 3D Box. For debugging
    static void outputBox3(std::ostream &os, const Box3 &box);
};

#endif // MESH_H_

