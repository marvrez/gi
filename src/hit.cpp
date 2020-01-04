#include "hit.h"

#include "ray.h"
#include "surface.h"

void Hit::RecordHit(double t, const Surface* s)
{
    this->s = s;
    this->t = t;
}

HitRecord Hit::GetRecord(const Ray& r)
{
    HitRecord hr;
    double t = this->t;
    const Surface* s = this->s;

    hr.t            = t;
    hr.position     = r.PositionAt(t);
    hr.normal       = s->NormalAt(hr.position);
    hr.material     = s->MaterialAt(hr.position);
    Vec3 uv         = s->UV(hr.position);
    hr.u = uv.u, hr.v = uv.v;

    return hr;
}
