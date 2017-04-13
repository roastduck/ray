#include "mesh.h"
#include "const.h"
#include "surface.h"

int Mesh::vertCnt = 0;

void Mesh::outputSurfaces(std::ostream &os)
{
    int baseCnt = vertCnt;
    auto surfaces = Surface::load(INPUT_FILE);
    float interval(1.0 / MESH_1D_NUM);
    for (const std::unique_ptr<Surface> &surf : surfaces)
    {
        for (int i = 0; i < MESH_1D_NUM + ! surf->uIsCircular(); i++)
            for (int j = 0; j < MESH_1D_NUM + ! surf->vIsCircular(); j++)
            {
                os << "v " << surf->position(i * interval, j * interval) << std::endl;
                vertCnt++;
            }
        for (int i = 0; i < MESH_1D_NUM; i++)
            for (int j = 0; j < MESH_1D_NUM; j++)
            {
                int _i(i < MESH_1D_NUM - 1 || ! surf->uIsCircular() ? i + 1 : 0);
                int _j(j < MESH_1D_NUM - 1 || ! surf->vIsCircular() ? j + 1 : 0);
                int p0(baseCnt + i * (MESH_1D_NUM + ! surf->vIsCircular()) + j + 1);
                int p1(baseCnt + i * (MESH_1D_NUM + ! surf->vIsCircular()) + _j + 1);
                int p2(baseCnt + _i * (MESH_1D_NUM + ! surf->vIsCircular()) + _j + 1);
                int p3(baseCnt + _i * (MESH_1D_NUM + ! surf->vIsCircular()) + j + 1);
                os << "f " << p0 << " " << p1 << " " << p2 << " " << p3 << std::endl;
            }
    }
}

void Mesh::outputBox3(std::ostream &os, const Box3 &box)
{
    os << "v " << Vec3(box.xMin, box.yMin, box.zMin) << std::endl;
    os << "v " << Vec3(box.xMin, box.yMin, box.zMax) << std::endl;
    os << "v " << Vec3(box.xMin, box.yMax, box.zMin) << std::endl;
    os << "v " << Vec3(box.xMin, box.yMax, box.zMax) << std::endl;
    os << "v " << Vec3(box.xMax, box.yMin, box.zMin) << std::endl;
    os << "v " << Vec3(box.xMax, box.yMin, box.zMax) << std::endl;
    os << "v " << Vec3(box.xMax, box.yMax, box.zMin) << std::endl;
    os << "v " << Vec3(box.xMax, box.yMax, box.zMax) << std::endl;
    os << "f " << vertCnt + 1 << " " << vertCnt + 2 << " " << vertCnt + 4 << " " << vertCnt + 3 << std::endl;
    os << "f " << vertCnt + 1 << " " << vertCnt + 5 << " " << vertCnt + 6 << " " << vertCnt + 2 << std::endl;
    os << "f " << vertCnt + 1 << " " << vertCnt + 5 << " " << vertCnt + 7 << " " << vertCnt + 3 << std::endl;
    os << "f " << vertCnt + 5 << " " << vertCnt + 7 << " " << vertCnt + 8 << " " << vertCnt + 6 << std::endl;
    os << "f " << vertCnt + 3 << " " << vertCnt + 7 << " " << vertCnt + 8 << " " << vertCnt + 4 << std::endl;
    os << "f " << vertCnt + 2 << " " << vertCnt + 6 << " " << vertCnt + 8 << " " << vertCnt + 4 << std::endl;
    vertCnt += 8;
}

