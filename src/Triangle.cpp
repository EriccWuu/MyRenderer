#include "Triangle.h"

Triangle::Triangle() = default;

Triangle::Triangle(vec4 v[3]) {
    for (int i = 3; i--; vert[i] = v[i]);
}

Triangle::Triangle(mat<3,4> v) {
    for (int i = 3; i--; vert[i] = v[i]);
}

std::vector<vec2> Triangle::bounding_box() {
    vec2 bottomleft, upperright;
    bottomleft.x = std::floor(std::min(vert[0].x, std::min(vert[1].x, vert[2].x)));
    bottomleft.y = std::floor(std::min(vert[0].y, std::min(vert[1].y, vert[2].y)));
    upperright.x = std::ceil(std::max(vert[0].x, std::max(vert[1].x, vert[2].x)));
    upperright.y = std::ceil(std::max(vert[0].y, std::max(vert[1].y, vert[2].y)));

    std::vector<vec2> bbox = {bottomleft, upperright};
    return bbox;
}

bool Triangle::isInsideTriangle(double x, double y) {
    vec3 p = {x, y, 1.0};
    // compute 2D barycentric coordinates
    vec3 a = vec3(vert[2].x-vert[0].x, vert[1].x-vert[0].x, vert[0].x-p.x);
    vec3 b = vec3(vert[2].y-vert[0].y, vert[1].y-vert[0].y, vert[0].y-p.y);
    vec3 u = cross(a, b);
    vec3 abc;
    if (std::abs(u.z) > 1e-2) abc = vec3(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    else abc = vec3(-1, 1, 1);

    if (abc.x < 0 || abc.y < 0 || abc.z < 0) return false;
    return true;
}