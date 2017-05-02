#ifndef KDTREE_H_
#define KDTREE_H_

#include <memory>
#include <vector>
#include "utils.h"

class KDTree
{
public:
    typedef ColoredRay data_t;

private:
    struct Node
    {
        enum split_t
        {
            LEAF = 0,
            BY_X = 1,
            BY_Y = 2,
            BY_Z = 3
        } splitBy;
        data_t data; /// Data associated with this node
        Box3 box; /// Bounding box of this node
        Node *l, *r; /// Children
        
        Node(const data_t &_data)
            : splitBy(LEAF), data(_data), box(KDTree::getPoint(data)), l(0), r(0) {}
    } *root;

    /** Build tree recursively
     * @param data : All points built into KD-tree. Will be modified
     * @param l, r, x : Current node is `x` which represent range [l, r)
     * @param splitBy : Splitting direction. This is cycling
     */
    void buildTree(std::vector<data_t> &data, int l, int r, Node *&x, Node::split_t splitBy);

    /** Get KNN recursively
     * @param x : Current node
     * @param res : Results
     * @param center, k : See getKNN
     */
    void getKNNImpl(const Node *x, std::vector<data_t> &res, const Vec3 &center, int k) const;

public:
    KDTree(std::vector<data_t> &data); /// `data` will be modified

    /** Get point that used in this kd-tree from data_t
     */
    static const Vec3 &getPoint(const data_t &data);

    /** find `k` nearest neighbors around `center`
     */
    std::vector<data_t> getKNN(const Vec3 &center, int k) const;
};

inline KDTree::KDTree(std::vector<data_t> &data) : root(0)
{
    buildTree(data, 0, data.size(), root, Node::BY_X);
}

inline const Vec3 &KDTree::getPoint(const KDTree::data_t &data)
{
    return data.ray.st;
}

inline std::vector<KDTree::data_t> KDTree::getKNN(const Vec3 &center, int k) const
{
    std::vector<KDTree::data_t> ret;
    ret.reserve(k);
    assert(root);
    getKNNImpl(root, ret, center, k);
    return ret;
}

#endif // KDTREE

