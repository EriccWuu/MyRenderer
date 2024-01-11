#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <vector>
#include "Matrix.h"

class Triangle {
    public:
    mat<3,4> vert = {};
    mat<3,4> norm = {};

    Triangle();
    Triangle(vec4 v[3]);
    Triangle(mat<3,4> v);
    std::vector<vec2> bounding_box();
    bool isInsideTriangle(double x, double y);
    vec3 barycentric2D(vec3 p);   // get the barycentric coordinates
};

#endif