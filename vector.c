#include <math.h>
#include "vector.h"

static Vector _add(Vector a, Vector b)
{
    Vector res = new_vector(new_point(0, 0, 0));

    res.point.x = a.point.x + b.point.x;
    res.point.y = a.point.y + b.point.y;
    res.point.z = a.point.z + b.point.z;

    return res;
}

static Vector _sub(Vector a, Vector b)
{
    Vector res = new_vector(new_point(0, 0, 0));

    res.point.x = a.point.x - b.point.x;
    res.point.y = a.point.y - b.point.y;
    res.point.z = a.point.z - b.point.z;

    return res;
}

static Vector _mul(Vector a, Vector b)
{
    Vector res = new_vector(new_point(0, 0, 0));

    res.point.x = a.point.x * b.point.x;
    res.point.y = a.point.y * b.point.y;
    res.point.z = a.point.z * b.point.z;

    return res;
}

static Vector _div(Vector a, Vector b)
{
    Vector res = new_vector(new_point(0, 0, 0));

    res.point.x = a.point.x / b.point.x;
    res.point.y = a.point.y / b.point.y;
    res.point.z = a.point.z / b.point.z;

    return res;
}

static Vector _mul_scalar(Vector a, float val)
{
    Vector res = new_vector(new_point(0, 0, 0));

    res.point.x = a.point.x * val;
    res.point.y = a.point.y * val;
    res.point.z = a.point.z * val;

    return res;
}

static Vector _div_scalar(Vector a, float val)
{
    Vector res = new_vector(new_point(0, 0, 0));

    res.point.x = a.point.x / val;
    res.point.y = a.point.y / val;
    res.point.z = a.point.z / val;

    return res;
}

static float _dot(Vector a, Vector b)
{
    return a.point.x * b.point.x + a.point.y * b.point.y + a.point.z * b.point.z;
}

static Vector _cross(Vector a, Vector b)
{
    Vector res = new_vector(new_point(0, 0, 0));

    res.point.x = a.point.y * b.point.z - a.point.z * b.point.y;
    res.point.y = a.point.z * b.point.x - a.point.x * b.point.z;
    res.point.z = a.point.x * b.point.y - a.point.y * b.point.x;

    return res;
}

static float _squared_length(Vector v)
{
    return v.point.x * v.point.x + v.point.y * v.point.y + v.point.z * v.point.z;
}

static float _length(Vector v)
{
    return sqrt(_squared_length(v));
}

static Vector _make_unit(Vector *v)
{
    float k = 1.0 / _length(*v);

    v->point.x *= k;
    v->point.y *= k;
    v->point.z *= k;

    return *v;
}

Vector new_vector(Point p)
{
    return (Vector) {
        .point = p,
        .color = { p.x, p.y, p.z },

        .add = _add,
        .sub = _sub,
        .mul = _mul,
        .div = _div,

        .mul_scalar = _mul_scalar,
        .div_scalar = _div_scalar,

        .dot = _dot,
        .cross = _cross,

        .length = _length,
        .squared_length = _squared_length,
        .make_unit = _make_unit
    };
}
