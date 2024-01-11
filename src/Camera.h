#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <math.h>
#include "Matrix.h"
#include "TGAImage.h"

class Camera {
public:
    vec4 position;
    vec4 direction;
    vec4 up;
    vec4 right;
    double fov;
    double aspectRatio;
    double width, height;
    int screen_w, screen_h;
    double near, far;
    int depth = 255;
    TGAImage frame;
    TGAImage zbuffer;

    Camera();
    Camera(vec3 pos, vec3 dir={0,0,-1}, vec3 u={0,1,0}, double w=100, double h=100, double fov=45, double r=1, double n=-0.5, double f=-50);

    void set(vec3 pos, vec3 dir, vec3 u, double w=100, double h=100, double fov=45, double r=1, double n=-0.5, double f=-50);
    mat4 lookAt();
    mat4 projection();
    mat4 viewport();
};

#endif