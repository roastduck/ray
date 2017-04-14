#ifndef BOXTREE_H_
#define BOXTREE_H_

#include <memory>
#include <utility>
#include "utils.h"
#include "intersection.h"

class Surface;

/** Tree of bounding box built on a surface
 *  Organized by surface parameters (u, v)
 */
class BoxTree
{
private:
    struct Node
    {
        enum SlideDir
        {
            BY_U = 0,
            BY_V = 1,
            LEAF = -1
        };

        std::unique_ptr<Node> l, r; /// children
        float u1, u2, v1, v2;
        Box3 box;
        SlideDir slideBy; // which direction are l and r slided by?

        Node(float _u1, float _u2, float _v1, float _v2, const Box3 &_box)
            : l(nullptr), r(nullptr), u1(_u1), u2(_u2), v1(_v1), v2(_v2), box(_box), slideBy(LEAF) {}
    };

    std::unique_ptr<Node> root;

public:
    BoxTree(const Surface &surf);

    /// Find intersection (box (leaf)*, (intersection point, ray parameter)) between surface and ray
    Optional< std::pair<const Node*, InterType> > findInter(const Ray &ray);

private:
    void buildTree(const Surface &surf, std::unique_ptr<Node> &node, float u1, float u2, float v1, float v2);

    /** Implement of findInter
     *  Asserting ray is intersecting with node->box
     *  @param inter : intersecting point with 
     */
    Optional< std::pair<const Node*, InterType> > findInterRecur(const Ray &ray, const std::unique_ptr<Node> &node, const InterType &inter);
};

#endif // BOXTREE_H_

