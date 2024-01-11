#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <string>
#include "Matrix.h"
#include "TGAImage.h"

class Model {
    std::vector<vec3> verts{};
    std::vector<vec3> norms{};
    std::vector<vec2> tex_coords{};
    std::vector<std::vector<vec3>> faces;
    std::vector<int> face_vert{};
    std::vector<int> face_tex{};
    std::vector<int> face_norm{};
    TGAImage diffusemap{};
    TGAImage normalmap{};
    TGAImage normalmap_tangent{};
    TGAImage specularmap{};
    void load_texture(const std::string filename, const std::string suffix, TGAImage &img);

public:
    Model(const std::string filename);
    int nverts() const;
    int nfaces() const;
    vec3 normal(const int iface, const int nthvert) const;
    vec3 vert(const int i) const;
    vec3 vert(const int iface, const int nthvert) const;
    std::vector<vec3> face(const int i) const;
    vec2 uv(const int iface, const int nthvert) const;
    TGAColor diffuse(vec2 uv) const;
    float specular(vec2 uv) const;
    vec3 normal(const vec2 &uv) const;
    vec3 normal_tangent(const vec2 &uv) const;
};

#endif