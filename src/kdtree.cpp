#include <cassert>
#include <algorithm>
#include "kdtree.h"

void KDTree::buildTree(std::vector<KDTree::data_t> &data, int l, int r, Node *&x, Node::split_t splitBy)
{
    assert(l < r);
    int mid((l + r) >> 1);
    auto cmp([splitBy](const data_t &dataL, const data_t &dataR) {
        const Vec3 &lhs(getPoint(dataL));
        const Vec3 &rhs(getPoint(dataR));
        switch (splitBy)
        {
        case Node::BY_X:
            return lhs.x < rhs.x;
        case Node::BY_Y:
            return lhs.y < rhs.y;
        case Node::BY_Z:
            return lhs.z < rhs.z;
        default:
            assert(false);
        };
    });
    std::nth_element(data.begin() + l, data.begin() + mid, data.begin() + r, cmp);
    assert(!x);
    x = new Node(data[mid]);
    if (l < mid)
    {
        buildTree(data, l, mid, x->l, Node::split_t(int(splitBy) % 3 + 1));
        x->box = merge(x->box, x->l->box);
    }
    if (mid + 1 < r)
    {
        buildTree(data, mid + 1, r, x->r, Node::split_t(int(splitBy) % 3 + 1));
        x->box = merge(x->box, x->r->box);
    }
}

void KDTree::getKNNImpl(const Node *x, std::vector<KDTree::data_t> &res, const Vec3 &center, int k) const
{
    float xD2 = (getPoint(x->data) - center).dist2();
    assert(int(res.size()) <= k);
    if (int(res.size()) < k || xD2 < (getPoint(res.back()) - center).dist2())
    {
        res.back() = x->data;
        for (int k = res.size() - 1; k >= 1 && xD2 < (getPoint(res[k - 1]) - center).dist2(); k--)
            std::swap(res[k], res[k - 1]);
    }

    Node *first = x->l, *second = x->r;
    float firstD2 = first ? first->box.dist2(center) : INF;
    float secondD2 = second ? second->box.dist2(center) : INF;
    if (firstD2 > secondD2)
        std::swap(firstD2, secondD2), std::swap(first, second);
    if (int(res.size()) < k || firstD2 < (getPoint(res.back()) - center).dist2())
        getKNNImpl(first, res, center, k);
    if (int(res.size()) < k || secondD2 < (getPoint(res.back()) - center).dist2()) // Must be calculated here
        getKNNImpl(second, res, center, k);
}

