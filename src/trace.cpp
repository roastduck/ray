#include <cmath>
#include <cassert>
#include <algorithm>
#include "trace.h"
#include "const.h"

Optional<Surface::SurfInterType> Trace::findInter(const std::vector< std::unique_ptr<Surface> > &surfaces, const Ray &ray)
{
    Optional<Surface::SurfInterType> ret;
    for (const auto &surf : surfaces)
    {
        auto interOpt = surf->findInter(ray);
        if (interOpt.isOk())
        {
            const auto &inter(interOpt.ok());
            if (!ret.isOk() || inter.t < ret.ok().t)
                ret = inter;
        }
    }
    return ret;
}

Vec3 Trace::reflectDir(const Vec3 &input, const Vec3 &norm)
{
    assert(fabs(norm.dist2() - 1) < EPS * EPS);
    assert(dot(input, norm) < 0);
    return Vec3(input + 2 * dot(input, norm) * norm);
}

Vec3 Trace::refrectDir(const Vec3 &input, const Vec3 &norm, float refrIdx)
{
    assert(fabs(norm.dist2() - 1) < EPS * EPS);
    assert(dot(input, norm) < 0);
    Vec3 horizontal(input + dot(input, norm) * norm);
    assert(fabs(dot(horizontal, norm)) < EPS);
    float sinThetaI = sqrtf(cross(input, norm).dist2() / input.dist2());
    float sinThetaT = sinThetaI / refrIdx;
    return Vec3(horizontal - sqrtf(horizontal.dist2()) / tanf(asin(sinThetaT)) * norm);
}

void Trace::correctFrontBack(const Vec3 &input, Vec3 &norm, float &refrIdx)
{
    norm = norm * (1.0f / sqrtf(norm.dist2())); // `norm` is unified
    if (dot(norm, input) > 0) // inside -> outside
        norm = -norm, refrIdx = 1.0f / refrIdx;
}

Vec3 Trace::colorFactor(const Vec3 &ray1, const Vec3 &ray2, const Surface::SurfInterType &inter)
{
    const Material &mat(inter.surf->material);
    float refrIdx = mat.Rn;
    Vec3 norm(inter.normal);
    correctFrontBack(ray1, norm, refrIdx);
    Vec3 reflection(reflectDir(ray1, norm));
    Vec3 refrection(refrectDir(ray1, norm, refrIdx));
    color_t ret(ENV_COLOR);
    ret += mat.Creflec * std::max(0.0f, mat.Kd * dot(ray2, norm));
    ret += mat.Creflec * std::max(0.0f, mat.Ks * dot(ray2, reflection));
    ret += mat.Ctrans * std::max(0.0f, mat.Ktd * dot(ray2, -norm));
    ret += mat.Ctrans * std::max(0.0f, mat.Kts * dot(ray2, refrection));
    return ret;
}

void Trace::trace(
    const std::vector< std::unique_ptr<Surface> > &surfaces,
    const ColoredRay &ray,
    int depth,
    const std::function<void(const Surface::SurfInterType&, const ColoredRay &ray)> &callback
)
{
    auto interOpt(findInter(surfaces, ray.ray));
    if (!interOpt.isOk()) return;
    auto inter(interOpt.ok());
    callback(inter, ray);
    if (!depth) return;
    if (ray.color.dist2() < sqr(0.004)) return;

    const Material &mat(inter.surf->material);
    float refrIdx = mat.Rn;
    Vec3 norm(inter.normal);
    correctFrontBack(ray.ray.dir, norm, refrIdx);

    float emitType = rand01() * (mat.Kd + mat.Ks + mat.Ktd + mat.Kts);
    ColoredRay emit;
    if (emitType < mat.Kd) // Diffuse reflection
    {
        emit.color = multiple(ray.color, mat.Creflec);
        emit.ray = randSemisphere(inter.pos, norm, 0);
    } else if ((emitType -= mat.Kd) < mat.Ks) // Specular reflection
    {
        Vec3 reflection(reflectDir(ray.ray.dir, norm));
        emit.color = multiple(ray.color, mat.Creflec);
        do
            emit.ray = randSemisphere(inter.pos, reflection, mat.Sn);
        while (dot(emit.ray.dir, norm) > 0);
    } else if ((emitType -= mat.Ks) < mat.Ktd) // Diffuse transition
    {
        emit.color = multiple(ray.color, mat.Ctrans);
        emit.ray = randSemisphere(inter.pos, -norm, 0);
    } else // Specular transition
    {
        Vec3 refrection(refrectDir(ray.ray.dir, norm, refrIdx));
        emit.color = multiple(ray.color, mat.Ctrans);
        do
            emit.ray = randSemisphere(inter.pos, refrection, mat.Sn);
        while (dot(emit.ray.dir, norm) < 0);
    }
    trace(surfaces, emit, depth - 1, callback);
}

