#include "mat4.h"

#include "vec3.h"

#include <math.h>

Mat4::Mat4(double a00, double a01, double a02, double a03,
           double a10, double a11, double a12, double a13,
           double a20, double a21, double a22, double a23,
           double a30, double a31, double a32, double a33)
    : a00(a00), a01(a01), a02(a02), a03(a03),
      a10(a10), a11(a11), a12(a12), a13(a13),
      a20(a20), a21(a21), a22(a22), a23(a23),
      a30(a30), a31(a31), a32(a32), a33(a33)
{
}

Mat4 Mat4::Translate(const Vec3& t) const
{
    return TranslationMatrix(t)*(*this);
}

Mat4 Mat4::Scale(const Vec3& s) const
{
    return ScaleMatrix(s)*(*this);
}

Mat4 Mat4::Rotate(const Vec3& axis, double angle_rad) const
{
    return RotationMatrix(axis, angle_rad)*(*this);
}

Vec3 Mat4::MultiplyPosition(const Vec3& p) const
{
    Vec3 v = {
        a00*p.x + a01*p.y + a02*p.z + a03,
        a10*p.x + a11*p.y + a12*p.z + a13,
        a20*p.x + a21*p.y + a22*p.z + a23
    };
    return v;
}

Vec3 Mat4::MultiplyDirection(const Vec3& d) const
{
    Vec3 v = {
        a00*d.x + a01*d.y + a02*d.z,
        a10*d.x + a11*d.y + a12*d.z,
        a20*d.x + a21*d.y + a22*d.z
    };
    return Normalized(v);
}

Mat4 IdentityMatrix()
{
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}

Mat4 TranslationMatrix(const Vec3& t)
{
    return {
        1, 0, 0, t.x,
        0, 1, 0, t.y,
        0, 0, 1, t.z,
        0, 0, 0, 1
    };
}

Mat4 ScaleMatrix(const Vec3& s)
{
    return {
        s.x, 0, 0, 0,
        0, s.y, 0, 0,
        0, 0, s.z, 0,
        0, 0,   0, 1
    };
}

Mat4 RotationMatrix(const Vec3& axis, double angle_rad)
{
    // https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
    Vec3 a = Normalized(axis);
    double s = sin(angle_rad), c = cos(angle_rad);
    double m = 1.0 - c;
    return {
        m*a.x*a.x + c, m*a.x*a.y - a.z*s, m*a.z*a.x + a.y*s, 0,
        m*a.x*a.y + a.z*s, m*a.y*a.y + c, m*a.y*a.z - a.x*s, 0,
        m*a.z*a.x - a.y*s, m*a.y*a.z + a.x*s, m*a.z*a.z + c, 0,
        0,                 0,                 0,             1
    };
}

Mat4 operator*(const Mat4& a, const Mat4& b)
{
    Mat4 m;
    m.a00 = a.a00*b.a00 + a.a01*b.a10 + a.a02*b.a20 + a.a03*b.a30;
    m.a01 = a.a00*b.a01 + a.a01*b.a11 + a.a02*b.a21 + a.a03*b.a31;
    m.a02 = a.a00*b.a02 + a.a01*b.a12 + a.a02*b.a22 + a.a03*b.a32;
    m.a03 = a.a00*b.a03 + a.a01*b.a13 + a.a02*b.a23 + a.a03*b.a33;
    m.a10 = a.a10*b.a00 + a.a11*b.a10 + a.a12*b.a20 + a.a13*b.a30;
    m.a11 = a.a10*b.a01 + a.a11*b.a11 + a.a12*b.a21 + a.a13*b.a31;
    m.a12 = a.a10*b.a02 + a.a11*b.a12 + a.a12*b.a22 + a.a13*b.a32;
    m.a13 = a.a10*b.a03 + a.a11*b.a13 + a.a12*b.a23 + a.a13*b.a33;
    m.a20 = a.a20*b.a00 + a.a21*b.a10 + a.a22*b.a20 + a.a23*b.a30;
    m.a21 = a.a20*b.a01 + a.a21*b.a11 + a.a22*b.a21 + a.a23*b.a31;
    m.a22 = a.a20*b.a02 + a.a21*b.a12 + a.a22*b.a22 + a.a23*b.a32;
    m.a23 = a.a20*b.a03 + a.a21*b.a13 + a.a22*b.a23 + a.a23*b.a33;
    m.a30 = a.a30*b.a00 + a.a31*b.a10 + a.a32*b.a20 + a.a33*b.a30;
    m.a31 = a.a30*b.a01 + a.a31*b.a11 + a.a32*b.a21 + a.a33*b.a31;
    m.a32 = a.a30*b.a02 + a.a31*b.a12 + a.a32*b.a22 + a.a33*b.a32;
    m.a33 = a.a30*b.a03 + a.a31*b.a13 + a.a32*b.a23 + a.a33*b.a33;
    return m;
}

Mat4 Transpose(const Mat4& m)
{
    return {
        m.a00, m.a10, m.a20, m.a30,
        m.a01, m.a11, m.a21, m.a31,
        m.a02, m.a12, m.a22, m.a32,
        m.a03, m.a13, m.a23, m.a33
    };
}

double Determinant(const Mat4& m)
{
    return m.a00*m.a11*m.a22*m.a33 - m.a00*m.a11*m.a23*m.a32 +
           m.a00*m.a12*m.a23*m.a31 - m.a00*m.a12*m.a21*m.a33 +
           m.a00*m.a13*m.a21*m.a32 - m.a00*m.a13*m.a22*m.a31 -
           m.a01*m.a12*m.a23*m.a30 + m.a01*m.a12*m.a20*m.a33 -
           m.a01*m.a13*m.a20*m.a32 + m.a01*m.a13*m.a22*m.a30 -
           m.a01*m.a10*m.a22*m.a33 + m.a01*m.a10*m.a23*m.a32 +
           m.a02*m.a13*m.a20*m.a31 - m.a02*m.a13*m.a21*m.a30 +
           m.a02*m.a10*m.a21*m.a33 - m.a02*m.a10*m.a23*m.a31 +
           m.a02*m.a11*m.a23*m.a30 - m.a02*m.a11*m.a20*m.a33 -
           m.a03*m.a10*m.a21*m.a32 + m.a03*m.a10*m.a22*m.a31 -
           m.a03*m.a11*m.a22*m.a30 + m.a03*m.a11*m.a20*m.a32 -
           m.a03*m.a12*m.a20*m.a31 + m.a03*m.a12*m.a21*m.a30;
}

Mat4 Inverse(const Mat4& m)
{
    Mat4 inv;
    double d = Determinant(m);
    inv.a00 = (m.a12*m.a23*m.a31 - m.a13*m.a22*m.a31 + m.a13*m.a21*m.a32 - m.a11*m.a23*m.a32 - m.a12*m.a21*m.a33 + m.a11*m.a22*m.a33) / d;
    inv.a01 = (m.a03*m.a22*m.a31 - m.a02*m.a23*m.a31 - m.a03*m.a21*m.a32 + m.a01*m.a23*m.a32 + m.a02*m.a21*m.a33 - m.a01*m.a22*m.a33) / d;
    inv.a02 = (m.a02*m.a13*m.a31 - m.a03*m.a12*m.a31 + m.a03*m.a11*m.a32 - m.a01*m.a13*m.a32 - m.a02*m.a11*m.a33 + m.a01*m.a12*m.a33) / d;
    inv.a03 = (m.a03*m.a12*m.a21 - m.a02*m.a13*m.a21 - m.a03*m.a11*m.a22 + m.a01*m.a13*m.a22 + m.a02*m.a11*m.a23 - m.a01*m.a12*m.a23) / d;
    inv.a10 = (m.a13*m.a22*m.a30 - m.a12*m.a23*m.a30 - m.a13*m.a20*m.a32 + m.a10*m.a23*m.a32 + m.a12*m.a20*m.a33 - m.a10*m.a22*m.a33) / d;
    inv.a11 = (m.a02*m.a23*m.a30 - m.a03*m.a22*m.a30 + m.a03*m.a20*m.a32 - m.a00*m.a23*m.a32 - m.a02*m.a20*m.a33 + m.a00*m.a22*m.a33) / d;
    inv.a12 = (m.a03*m.a12*m.a30 - m.a02*m.a13*m.a30 - m.a03*m.a10*m.a32 + m.a00*m.a13*m.a32 + m.a02*m.a10*m.a33 - m.a00*m.a12*m.a33) / d;
    inv.a13 = (m.a02*m.a13*m.a20 - m.a03*m.a12*m.a20 + m.a03*m.a10*m.a22 - m.a00*m.a13*m.a22 - m.a02*m.a10*m.a23 + m.a00*m.a12*m.a23) / d;
    inv.a20 = (m.a11*m.a23*m.a30 - m.a13*m.a21*m.a30 + m.a13*m.a20*m.a31 - m.a10*m.a23*m.a31 - m.a11*m.a20*m.a33 + m.a10*m.a21*m.a33) / d;
    inv.a21 = (m.a03*m.a21*m.a30 - m.a01*m.a23*m.a30 - m.a03*m.a20*m.a31 + m.a00*m.a23*m.a31 + m.a01*m.a20*m.a33 - m.a00*m.a21*m.a33) / d;
    inv.a22 = (m.a01*m.a13*m.a30 - m.a03*m.a11*m.a30 + m.a03*m.a10*m.a31 - m.a00*m.a13*m.a31 - m.a01*m.a10*m.a33 + m.a00*m.a11*m.a33) / d;
    inv.a23 = (m.a03*m.a11*m.a20 - m.a01*m.a13*m.a20 - m.a03*m.a10*m.a21 + m.a00*m.a13*m.a21 + m.a01*m.a10*m.a23 - m.a00*m.a11*m.a23) / d;
    inv.a30 = (m.a12*m.a21*m.a30 - m.a11*m.a22*m.a30 - m.a12*m.a20*m.a31 + m.a10*m.a22*m.a31 + m.a11*m.a20*m.a32 - m.a10*m.a21*m.a32) / d;
    inv.a31 = (m.a01*m.a22*m.a30 - m.a02*m.a21*m.a30 + m.a02*m.a20*m.a31 - m.a00*m.a22*m.a31 - m.a01*m.a20*m.a32 + m.a00*m.a21*m.a32) / d;
    inv.a32 = (m.a02*m.a11*m.a30 - m.a01*m.a12*m.a30 - m.a02*m.a10*m.a31 + m.a00*m.a12*m.a31 + m.a01*m.a10*m.a32 - m.a00*m.a11*m.a32) / d;
    inv.a33 = (m.a01*m.a12*m.a20 - m.a02*m.a11*m.a20 + m.a02*m.a10*m.a21 - m.a00*m.a12*m.a21 - m.a01*m.a10*m.a22 + m.a00*m.a11*m.a22) / d;
    return inv;
}

Mat4 FrustumMatrix(double left, double right, double bottom, double top, double near, double far)
{
    double t1 = 2*near, t2 = right - left, t3 = top - bottom, t4 = far - near;
    return {
        t1/t2, 0, (right+left)/t2, 0,
        0, t1/t3, (top+bottom)/t3, 0,
        0, 0, -(far+near)/t4, -t1*far/t4,
        0, 0, -1, 0
    };
}

Mat4 OrthographicMatrix(double left, double right, double bottom, double top, double z_near, double z_far)
{
    // Map x from [left, right] to [-1, +1]
    //     y from [bottom, top] to [-1, +1]
    //     z from [z_near, z_far]      to [-1, +1]
    return {
        2.0 / (right - left), 0, 0, -(right + left) / (right - left),
        0, 2.0 / (top - bottom), 0, -(top + bottom) / (top - bottom),
        0, 0, -2.0 / (z_far - z_near), -(z_far + z_near) / (z_far - z_near),
        0, 0, 0, 1
    };
}

Mat4 PerspectiveMatrix(double vfov_rad, double aspect, double z_near, double z_far)
{
    double f = tan(M_PI_2 - 0.5*vfov_rad), range_inv = 1.0 / (z_near - z_far);
    return {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (z_near+z_far)*range_inv, 2.0*(z_near*z_far)*range_inv,
        0, 0, -1, 0
    };
}

Mat4 LookAtMatrix(const Vec3& lookfrom, const Vec3& lookat, const Vec3& up)
{
    // orthonormal basis
    Vec3 w = Normalized(lookat - lookfrom);
    Vec3 u = Normalized(Cross(w, up));
    Vec3 v = Normalized(Cross(u, w));
    Mat4 m = {
        u.x, v.x, w.x, 0,
        u.y, v.y, w.y, 0,
        u.z, v.z, w.z, 0,
        0  , 0  , 0  , 1
    };
    return Transpose(Inverse(m.Translate(lookfrom)));
}
