#ifndef __GL_H__
#define __GL_H__

#include "TGAImage.h"
#include "Matrix.h"
#include "Shader.h"

struct Triangle {
    public:
    mat<3,4> vert = {};
    mat<3,4> norm = {};

    Triangle() = default;

    Triangle(vec4 v[3]) {
        for (int i = 3; i--; vert[i] = v[i]);
    }

    Triangle(mat<3,4> v) {
        for (int i = 3; i--; vert[i] = v[i]);
    }
};

// Basic color defination
const TGAColor white = TGAColor({255, 255, 255, 255});
const TGAColor red   = TGAColor({0, 0, 255, 255});
const TGAColor green = TGAColor({0, 255, 0, 255});
const TGAColor blue  = TGAColor({255, 0, 0, 255});

// Model
mat4 scale(double s);
mat4 translate(vec3 t);
mat4 rotateX(double angle);
mat4 rotateY(double angle);
mat4 rotateZ(double angle);
mat4 rotate(vec3 n, double angle);

// View
mat4 lookAt(vec3 position, vec3 direction, vec3 up);

// Projection
mat4 projection(double fov, double aspectRatio, double near, double far);

// Viewport
mat4 viewport(int screen_w, int screen_h);

// Find bounding box of triangle
std::vector<vec2> bounding_box(Triangle t);

// Check point whether inside the triangle
bool isInsideTriangle(double x, double y, Triangle t);

// compute 2D barycentric coordinates
static std::tuple<double, double, double> computeBarycentric(double x, double y, const vec3 *v);

// Perspective-Correct Interpolation
double interpLine(double a1, double a2, double z1, double z2, double s);
double interpZ(double x, double y, const vec4 *v);
double interp(double x, double y, const double *I, const vec4 *v);

// draw line
void drawline(int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color);

// draw bounding box
void drawBoundingBox(std::vector<vec2> bbox, TGAImage &image, TGAColor color);

// draw triangle
void drawTriangle_mesh(Triangle t,  TGAImage &image, TGAColor color = white);
void drawTriangle(Triangle t, IShader &s, TGAImage &image, double *zbuffer);

#endif