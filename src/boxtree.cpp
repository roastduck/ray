#include <cassert>
#include "const.h"
#include "surface.h"
#include "boxtree.h"

BoxTree::BoxTree(const Surface &surf)
{
    assert(! root);
    root = std::unique_ptr<Node>(new Node(surf.xyzMinMax(0, 1, 0, 1)));
    buildTree(surf, root, 0, 1, 0, 1);
}

void BoxTree::buildTree(const Surface &surf, std::unique_ptr<Node> &node, float u1, float u2, float v1, float v2)
{
    assert(node);
    assert(u1 < u2 && v1 < v2);
    float um = (u1 + u2) * 0.5, vm = (v1 + v2) * 0.5;
    Box3 bu1, bu2, bv1, bv2;
    if (MESH_1D_NUM * (u1 - u2) >= 1)
    {
        bu1 = surf.xyzMinMax(u1, um, v1, v2), bu2 = surf.xyzMinMax(um, u2, v1, v2);
        assert(bu1.size() + bu2.size() < node->box.size() + 1e-5);
    }
    if (MESH_1D_NUM * (v1 - v2) >= 1)
    {
        bv1 = surf.xyzMinMax(u1, u2, v1, vm), bv2 = surf.xyzMinMax(u1, u2, vm, v2);
        assert(bv1.size() + bv2.size() < node->box.size() + 1e-5);
    }
    if (MESH_1D_NUM * (u1 - u2) >= 1 && (MESH_1D_NUM * (v1 - v2) < 1 || bu1.size() + bu2.size() < bv1.size() + bv2.size()))
    {
        node->l = std::unique_ptr<Node>(new Node(bu1)), node->r = std::unique_ptr<Node>(new Node(bu2));
        node->slideBy = Node::BY_U;
        buildTree(surf, node->l, u1, um, v1, v2);
        buildTree(surf, node->r, um, u2, v1, v2);
    } else if (MESH_1D_NUM * (v1 - v2) >= 1)
    {
        node->l = std::unique_ptr<Node>(new Node(bv1)), node->r = std::unique_ptr<Node>(new Node(bv2));
        node->slideBy = Node::BY_V;
        buildTree(surf, node->l, u1, u2, v1, vm);
        buildTree(surf, node->r, u1, u2, vm, v2);
    }
}

Optional< std::pair<const Box3*, InterType> > BoxTree::findInter(const Ray &ray)
{
    assert(root);
    assert(root->slideBy != Node::LEAF);
    Optional<InterType> inter(intersec(root->box, ray));
    if (! inter.isOk()) return None();
    return findInterRecur(ray, root, inter.ok());
}

Optional< std::pair<const Box3*, InterType> > BoxTree::findInterRecur(const Ray &ray, const std::unique_ptr<Node> &node, const InterType &inter)
{
    assert(intersec(node->box, ray).isOk());
    if (node->slideBy == Node::LEAF)
        return std::pair<const Box3*, InterType>(&(node->box), inter);
    assert(node->l && node->r);
    Optional< std::pair<Vec3, float> > iL(intersec(node->l->box, ray)), iR(intersec(node->r->box, ray));
    if (! iL.isOk() && ! iR.isOk())
        return None();
    Optional< std::pair< const Box3*, std::pair<Vec3, float> > > ret;
    if (iL.isOk() && (! iR.isOk() || iL.ok().second < iR.ok().second))
        return (ret = findInterRecur(ray, node->l, iL.ok())).isOk() ? ret : iR.isOk() ? findInterRecur(ray, node->r, iR.ok()) : None();
    else // ! iL.isOk() || iR.isOk() && iL.ok().second >= iR.ok().second
        return (ret = findInterRecur(ray, node->r, iR.ok())).isOk() ? ret : iL.isOk() ? findInterRecur(ray, node->l, iL.ok()) : None();
}

