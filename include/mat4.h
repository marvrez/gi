#ifndef MAT4_H
#define MAT4_H

struct Vec3;

struct Mat4 {
    // data is stored in row-major order
    union {
        double data[16];
        struct { double a00, a01, a02, a03,
                        a10, a11, a12, a13,
                        a20, a21, a22, a23,
                        a30, a31, a32, a33;
        };
    };
    double& operator[](int i) { return data[i]; }

    Mat4() = default;
    Mat4(double a00, double a01, double a02, double a03, 
         double a10, double a11, double a12, double a13,
         double a20, double a21, double a22, double a23,
         double a30, double a31, double a32, double a33);

    Mat4 Translate(const Vec3& t) const;
    Mat4 Scale(const Vec3& s) const;
    Mat4 Rotate(const Vec3& axis, double angle_rad) const;

    Vec3 MultiplyPosition(const Vec3& p) const;
    Vec3 MultiplyDirection(const Vec3& d) const;
};

Mat4 IdentityMatrix();
Mat4 TranslationMatrix(const Vec3& t);
Mat4 ScaleMatrix(const Vec3& s);
Mat4 RotationMatrix(const Vec3& axis, double angle_rad);
Mat4 operator*(const Mat4& a, const Mat4& b);
Mat4 Transpose(const Mat4& m);
double Determinant(const Mat4& m);
Mat4 Inverse(const Mat4& m);

Mat4 FrustumMatrix(double left, double right, double bottom, double top, double near, double far);
Mat4 OrthographicMatrix(double left, double right, double bottom, double top, double z_near=-1, double z_far=1);
Mat4 PerspectiveMatrix(double vfov_rad, double aspect, double z_near, double z_far);
Mat4 LookAtMatrix(const Vec3& lookfrom, const Vec3& lookat, const Vec3& up);

#endif
