/** Calculate intersectins
 */

#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "utils.h"

class Surface;

typedef std::pair<Vec3, float> InterType;

struct SurfInterType
{
    const Surface *surf;
    float t, u, v;
    Vec3 pos, normal;
    SurfInterType() : surf(0) {}
    SurfInterType(const Surface *_surf, float _t, float _u, float _v, const Vec3 &_pos, const Vec3 &_normal)
        : surf(_surf), t(_t), u(_u), v(_v), pos(_pos), normal(_normal) {}
};

/** These functinos find intersection between yz/xz/xy-plane and ray
 *  Asserting the ray intersecting with the extended infinte plane
 *  @param x,y,z : Plane position
 *  @param box : Plane size
 *  @return : (Intersection point, Ray parameter)
 */
Optional<InterType> intersecYZ(float x, const Box2 &box, const Ray &ray);
Optional<InterType> intersecXZ(float y, const Box2 &box, const Ray &ray);
Optional<InterType> intersecXY(float z, const Box2 &box, const Ray &ray);

/** Find intersection between box and ray
 *  @return : (Intersection point, Ray parameter)
 */
Optional<InterType> intersec(const Box3 &box, const Ray &ray);

/** Find intersection with a surface based on box intersection
 *  @param t, u, v : Initial values
 *  @param p0 : Initial intersection
 */
Optional<SurfInterType> intersec(float t, float u, float v, const Vec3 &p0, const Surface &surf, const Ray &ray);

#endif // INTERSECTION_H_

