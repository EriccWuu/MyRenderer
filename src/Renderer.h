#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Model.h"
#include "TGAImage.h"
#include "Matrix.h"
#include "GL.h"
#include "Camera.h"
#include "Shader.h"

class Renderer {
    double *zbuffer;
    Camera camera;

public:
    TGAImage frame_img;
    TGAImage zbuffer_img;
    Renderer();
    ~Renderer();

    void setCamera(vec3 pos, vec3 dir = {0.f, 0.f, -1.f}, vec3 up = {0.f, 1.f, 0.f}, double w=100, double h=100, double fov=45, double r=1, double n=-0.5, double f=-50);
    void initZbuffer();
    Camera& cam();
    Model* mod();
    double* zbuf();
    void draw(const Model *model, IShader &shader, TGAImage &image, bool DRAW_MESH=false);
    void draw_zbuffer();
};

#endif