#ifndef __SHADER_H__
#define __SHADER_H__

#include "Model.h"

class IShader {
public:
    const Model *model;
    mat<2,3> varying_uv;
    mat<3,3> varying_norm;
    mat<3,3> varying_vert;
    mat4 MODEL;
    mat4 VIEW;
    mat4 PROJECTION;
    mat4 VIEWPORT;
    mat4 MV, MV_IT; // VIEW * MODEL, (VIEW * MODEL).invert_transpose()
    mat4 MVP, MVP_I; // PROJECTION * VIEW * MODEL, (PROJECTION * VIEW * MODEL).invert()

    IShader(): model(nullptr) {}

    static TGAColor sample2D(const TGAImage &img, vec2 &uvf) {
        return img.get(uvf[0] * img.width(), uvf[1] * img.height());
    }

    void setModel(const Model *m) {
        model = m;
    }

    virtual vec4 vertex(const int iface, const int nthvert) = 0;
    virtual bool fragment(const vec3 bar, TGAColor &gl_Fragcolor) = 0;
};

class Shader: public IShader {
public:
    vec3 uniform_light;
    const double *shadowmap;
    mat4 uniform_Mshadow;
    int height, width;

    TGAImage debug_img = TGAImage(1024, 1024, TGAImage::RGB);

    vec4 vertex(const int iface, const int nthvert) override;
    bool fragment(const vec3 bar, TGAColor &gl_Fragcolor) override;
};

class DepthShader: public IShader {
public:
    vec4 vertex(const int iface, const int nthvert) override;
    bool fragment(const vec3 bar, TGAColor &gl_Fragcolor) override;
};

#endif