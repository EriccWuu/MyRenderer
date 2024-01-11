#include <iostream>
#include <sstream>
#include "Model.h"

Model::Model(const std::string filename) {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) {
        std::cerr << "Can't open file" << filename << '\n';
        return;
    }
    std:: string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            vec3 v;
            for (int i = 0; i < 3; i ++) iss >> v[i];
            verts.push_back(v);
        }
        else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            vec3 n;
            for (int i = 0; i < 3; i ++) iss >> n[i];
            norms.push_back(n.normalize());
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            vec2 uv;
            for (int i = 0; i < 2; i ++) iss >> uv[i];
            uv.y = 1 - uv.y;    // flip in y = 0.5
            tex_coords.push_back(uv);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<vec3> face;
            int f, t, n;
            iss >> trash;
            int cnt = 0;
            while (iss >> f >> trash >> t >> trash >> n) {
                face_vert.push_back(--f);
                face_tex.push_back(--t);
                face_norm.push_back(--n);
                face.push_back(vec3(f, t, n));
                cnt ++;
            }
            faces.push_back(face);
            if (cnt != 3) {
                std::cerr << "Error: the obj file is supposed to be triangulated." << std::endl;
                return ;
            }
        }
    }
    std::cerr << "- Model Info:" << " #v#" << nverts() << " #f#" << nfaces() << " #vt#" <<tex_coords.size() << " #vn#" << norms.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffusemap);
    // load_texture(filename, "_nm.tga", normalmap);
    load_texture(filename, "_nm_tangent.tga", normalmap_tangent);
    load_texture(filename, "_spec.tga", specularmap);
}

void Model::load_texture(const std::string filename, const std::string suffix, TGAImage &img) { 
    size_t dot = filename.find_last_of(".");
    if (dot == std::string::npos) return;
    std::string texfile = filename.substr(0, dot) + suffix;
    std::cerr << "Loading texture file \"" << texfile << "\"...\n";
    std::cerr << (img.read_tga_file(texfile.c_str()) ? "- Load successfully" : "- Fail to load.") << std::endl;
}

int Model::nverts() const { return verts.size(); }

int Model::nfaces() const { return face_vert.size() / 3;}

std::vector<vec3> Model::face(const int i) const { return faces[i]; }

vec3 Model::normal(const int iface, const int nthvert) const { return norms[face_norm[iface * 3 + nthvert]]; }

vec3 Model::vert(const int i) const { return verts[i]; }

vec3 Model::vert(const int iface, const int nthvert) const { return verts[face_vert[iface * 3 + nthvert]]; }

vec2 Model::uv(const int iface, const int nthvert) const { 
    return tex_coords[faces[iface][nthvert][1]]; 
    // return tex_coords[face_tex[iface*3 + nthvert]]; 
}

TGAColor Model::diffuse(vec2 uv) const {
    vec2 uv_(uv.x*diffusemap.width(), uv.y*diffusemap.height());
    return diffusemap.get(uv_.x, uv_.y);
}

float Model::specular(vec2 uv) const {
    vec2 uv_(uv.x*specularmap.width(), uv.y*specularmap.height());
    return specularmap.get(uv_.x, uv_.y)[0]/1.f;
}

vec3 Model::normal(const vec2 &uv) const { 
    TGAColor c = normalmap.get(uv[0]*normalmap.width(), uv[1]*normalmap.height());
    return vec3{(double)c[2], (double)c[1], (double)c[0]} * 2.f / 255.f - vec3{1.f, 1.f, 1.f};  // (0, 255) -> (-1, 1)
}

vec3 Model::normal_tangent(const vec2 &uv) const { 
    TGAColor c = normalmap_tangent.get(uv[0]*normalmap_tangent.width(), uv[1]*normalmap_tangent.height());
    return vec3((double)c[2], (double)c[1], (double)c[0]) * 2.f / 255.f - vec3{1.f, 1.f, 1.f};  // (0, 255) -> (-1, 1)
}