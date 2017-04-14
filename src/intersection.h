/** Calculate intersectins
 */

#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "utils.h"

typedef std::pair<Vec3, float> InterType;

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

#endif // INTERSECTION_H_

