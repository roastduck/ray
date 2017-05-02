#ifndef PHOTON_MAP_H_
#define PHOTON_MAP_H_

#include <cassert>
#include <memory>
#include "utils.h"
#include "kdtree.h"

class Surface;

/** Photon Map
 *  Use in order: 1. Add multiple rays
 *                2. Build tree
 *                3. Find KNN multiple times
 */
class PhotonMap
{
private:
    const Surface &surf;
    std::unique_ptr< std::vector<ColoredRay> > rawData;
    std::unique_ptr<KDTree> tree;

public:
    PhotonMap(const Surface &_surf);
    void addRay(const ColoredRay &ray);
    void buildTree();
    std::vector<ColoredRay> getKNN(const Vec3 &center, int k) const;
};

inline void PhotonMap::addRay(const ColoredRay &ray)
{
    assert(rawData && !tree);
    rawData->push_back(ray);
}

#endif // PHOTON_MAP_H_

