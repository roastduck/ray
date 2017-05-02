#include "surface.h"
#include "photonmap.h"

PhotonMap::PhotonMap(const Surface &_surf)
    : surf(_surf), rawData(new std::vector<ColoredRay>()), tree(nullptr) {}

void PhotonMap::buildTree()
{
    assert(rawData && !tree);
    assert(!rawData->empty());
    tree = std::unique_ptr<KDTree>(new KDTree(*rawData));
    rawData = nullptr;
}

std::vector<ColoredRay> PhotonMap::getKNN(const Vec3 &center, int k) const
{
    assert(!rawData && tree);
    return tree->getKNN(center, k);
}

