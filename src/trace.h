#ifndef TRACE_H_
#define TRACE_H_

#include <vector>
#include <functional>
#include "utils.h"
#include "surface.h"
#include "material.h"

/** Perform tracing
 *  This is a static class
 */
class Trace
{
private:
    static void correctFrontBack(const Vec3 &input, Vec3 &norm, float &refrIdx);

public:
    /// Find nearest intersection on all surfaces
    static Optional<Surface::SurfInterType> findInter(const std::vector< std::unique_ptr<Surface> > &surfaces, const Ray &ray);

    /// Calculate reflection direction
    /// @param input : Ray goes in
    /// @param norm : UNIFIED normal vector points OPPOSITE to input
    static Vec3 reflectDir(const Vec3 &input, const Vec3 &norm);

    /// Calculate refrection direction
    /// @param input : Ray goes in
    /// @param norm : UNIFIED normal vector points OPPOSITE to input
    /// @param refrIdx : Relative refrection index
    static Optional<Vec3> refrectDir(const Vec3 &input, const Vec3 &norm, float refrIdx);

    /// Calculate light attenuation
    static Vec3 colorFactor(const Vec3 &ray1, const Vec3 &ray2, const Surface::SurfInterType &inter);

    /// Monte Carlo photon trace
    /// @param surfaces : All surfaces
    /// @param ray : Incoming ray
    /// @param depth : Recursion depth
    /// @param callback : fn(intersection, incoming ray, depth left). Return false to stop tracing
    static void trace(
        const std::vector< std::unique_ptr<Surface> > &surfaces,
        const ColoredRay &ray,
        int depth,
        const std::function<bool(const Surface::SurfInterType&, const ColoredRay &ray, int depth)> &callback
    );
};

#endif // TRACE_H_

