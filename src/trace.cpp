#include <cmath>
#include <cassert>
#include <algorithm>
#include "trace.h"
#include "const.h"

Optional<SurfInterType> Trace::findInter(const std::vector< std::unique_ptr<Surface> > &surfaces, const Ray &ray)
{
    Optional<SurfInterType> ret;
    for (const auto &surf : surfaces)
    {
        if (surf->isLightSource())
            continue; // Visible light source can produce much noise
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
    return input - 2 * dot(input, norm) * norm;
}

Optional<Vec3> Trace::refrectDir(const Vec3 &input, const Vec3 &norm, float refrIdx)
{
    float sinThetaI = sqrtf(cross(input, norm).dist2() / input.dist2());
    if (sinThetaI < EPS) return -norm;
    float sinThetaT = sinThetaI / refrIdx;
    if (sinThetaT > 1) return None(); // Total refrection
    assert(fabs(norm.dist2() - 1) < EPS * EPS);
    assert(dot(input, norm) < 0);
    Vec3 horizontal(input - dot(input, norm) * norm);
    assert(fabs(dot(horizontal, norm)) < EPS);
    auto ret = horizontal - sqrtf(horizontal.dist2()) / tanf(asin(sinThetaT)) * norm;
    assert(fabsf(sqrtf(cross(ret, norm).dist2() / ret.dist2()) - sinThetaT) < EPS);
    return ret;
}

void Trace::correctFrontBack(const Vec3 &input, Vec3 &norm, float &refrIdx)
{
    norm *= 1.0f / sqrtf(norm.dist2()); // `norm` is unified
    if (dot(norm, input) > 0) // inside -> outside
        norm = -norm, refrIdx = 1.0f / refrIdx;
}

Vec3 Trace::colorFactor(const Vec3 &ray1, const Vec3 &ray2, const SurfInterType &inter)
{
    const Material &mat(*(inter.surf->material));
    float refrIdx = mat.Rn;
    Vec3 norm(inter.normal);
    correctFrontBack(ray1, norm, refrIdx);
    assert(fabs(norm.dist2() - 1) < EPS * EPS);
    
    color_t ret(0, 0, 0);
    Vec3 uniRay1 = ray1 * (1.0f / sqrtf(ray1.dist2()));
    Vec3 uniRay2 = ray2 * (1.0f / sqrtf(ray2.dist2()));
    if (dot(ray2, norm) < 0) // reflection
    {
        Vec3 reflection(reflectDir(uniRay1, norm));
        assert(fabsf(reflection.dist2() - 1) < EPS);
        ret = mat.getCreflec(inter.u, inter.v) * (
            mat.Kd * -dot(uniRay1, norm) +
            mat.Ks * powf(std::max(0.0f, -dot(uniRay2, reflection)), mat.Sn)
        );
    } else { // refrection
        auto refrection(refrectDir(ray1, norm, refrIdx));
        if (refrection.isOk()) refrection.ok() *= (1.0f / sqrtf(refrection.ok().dist2()));
        ret = mat.Ctrans * (
            mat.Ktd * -dot(uniRay1, norm) +
            (refrection.isOk() ? mat.Kts * mat.Kts * powf(std::max(0.0f, -dot(uniRay2, refrection.ok())), mat.Sn) : 0)
        );
    }
    assert(ret.x >= 0 && ret.y >= 0 && ret.z >= 0);
    return ret;
}

void Trace::trace(
    std::default_random_engine &urng,
    const std::vector< std::unique_ptr<Surface> > &surfaces,
    const ColoredRay &ray,
    int depth,
    bool isSight,
    const std::function<bool(const SurfInterType&, const ColoredRay &ray, int depth)> &callback
)
{
    auto interOpt(findInter(surfaces, ray.ray));
    if (!interOpt.isOk()) return;
    auto inter(interOpt.ok());
    if (!callback(inter, ray, depth)) return;
    if (!depth) return;
    if (ray.color.dist2() < sqr(0.004)) return;

    const Material &mat(*(inter.surf->material));
    float refrIdx = mat.Rn;
    Vec3 norm(inter.normal);
    correctFrontBack(ray.ray.dir, norm, refrIdx);
    assert(fabs(norm.dist2() - 1) < EPS * EPS);

    auto refrectionOpt(refrectDir(ray.ray.dir, norm, refrIdx));

    float _Kts = refrectionOpt.isOk() ? mat.Kts : 0;
    if (mat.Kd + mat.Ks + mat.Ktd + _Kts < EPS) return;
    int emitType = std::discrete_distribution<int>({mat.Kd, mat.Ks, mat.Ktd, _Kts})(urng);
    ColoredRay emit;
    switch (emitType)
    {
    case 0: { // Diffuse reflection
        if (isSight)
        {
            emit.color = multiple(ray.color, mat.getCreflec(inter.u, inter.v));
            emit.ray = randSemisphere(urng, inter.pos, norm, 1);
        } else {
            emit.color = multiple(ray.color, mat.getCreflec(inter.u, inter.v)) * (fabsf(dot(ray.ray.dir, norm)) / sqrtf(ray.ray.dir.dist2()));
            emit.ray = randSemisphere(urng, inter.pos, norm, 0);
        }
        break;
    }
    case 1: { // Specular reflection
        Vec3 reflection(reflectDir(ray.ray.dir, norm));
        emit.color = multiple(ray.color, mat.getCreflec(inter.u, inter.v));
        do
            emit.ray = randSemisphere(urng, inter.pos, reflection, mat.Sn);
        while (dot(emit.ray.dir, norm) <= 0);
        break;
    }
    case 2: { // Diffuse transition
        if (isSight)
        {
            emit.color = multiple(ray.color, mat.Ctrans);
            emit.ray = randSemisphere(urng, inter.pos, -norm, 1);
        } else
        {
            emit.color = multiple(ray.color, mat.Ctrans) * (fabsf(dot(ray.ray.dir, norm)) / sqrtf(ray.ray.dir.dist2()));
            emit.ray = randSemisphere(urng, inter.pos, -norm, 0);
        }
        break;
    }
    case 3: { // Specular transition
        Vec3 refrection(refrectionOpt.ok());
        emit.color = multiple(ray.color, mat.Ctrans);
        do
            emit.ray = randSemisphere(urng, inter.pos, refrection, mat.Sn);
        while (dot(emit.ray.dir, norm) >= 0);
        break;
    }
    default:
        assert(false);
    }
    emit.color *= mat.Kd + mat.Ks + mat.Ktd + _Kts; // Others are absorbed
    trace(urng, surfaces, emit, depth - 1, isSight, callback);
}

