#include "Shader.h"
#include <algorithm>

/***********************************************************************************
********                                Shader                              ********
***********************************************************************************/
vec4 Shader::vertex(const int iface, const int nthvert) {
    vec4 gl_vertex;
    vec3 norm = (MV_IT * embed<4>(model->normal(iface, nthvert))).xyz().normalize();    // Transform normals
    varying_norm.setCol(nthvert, norm);
    varying_uv.setCol(nthvert, model->uv(iface, nthvert));
    gl_vertex = MV * vec4(model->vert(iface, nthvert), 1.0);  // Now in view space
    varying_vert.setCol(nthvert, gl_vertex.xyz());
    // std::cout << PROJECTION * gl_vertex << '\n';
    return gl_vertex; // Now in screen space
}

// This code defines the fragment shader function for a rendering pipeline.
// It performs various calculations for texture mapping, normal mapping, and bling-phong shading.
// The final color of the fragment is determined by the diffuse component and the intensity of the light.
// The code also includes tangent space calculations for normal mapping.
// The resulting color is assigned to the output variable gl_Fragcolor.
bool Shader::fragment(const vec3 bar, TGAColor &gl_Fragcolor) {
    vec2 uv = varying_uv*bar;

    // // Texture Map:
    // vec3 n = varying_norm * bar;
    // float intensity = std::max(-uniform_light*n, 0.0);
    // gl_Fragcolor = model->diffuse(uv)*intensity;

    // // Texture Map + Normal Mapping:
    // vec3 n = slice<3>(MV_IT * embed<4>(model->normal(uv))).normalize();
    // vec3 l = -uniform_light;
    // float intensity = std::max(n*l, 0.0);
    // gl_Fragcolor = model->diffuse(uv)*intensity;

    // // Texture Map + Normal Map + bling-phong:
    // vec3 n = (MV_IT * embed<4>(model->normal(uv))).xyz().normalize(); // Transfer normals in normal map
    // vec3 l = -uniform_light;
    // vec3 r = (n*(n*l*2.f) - l).normalize(); // Reflected light
    // float ambi = 0.01; // Ambient term
    // float diff = std::max(0.0, n*l);    // Diffuse term
    // float spec = 0.6*pow(std::max(r.z, 0.0), model->specular(uv));  // Specular term, r.z = cos(theta), theta is the angle between z axis and r.
    // float intensity = ambi + diff + spec;
    // TGAColor c = model->diffuse(uv);
    // gl_Fragcolor = c;
    // for (int i = 0; i < 3; i ++) gl_Fragcolor[i] = std::min<float>(5 + c[i]*(diff + 0.6*spec), 255);
    // // for (int i = 0; i < 3; i ++) gl_Fragcolor[i] = std::min<float>(c[i]*intensity, 255);

    // Texture Map + Normal Map (Tangent Space) + bling-phong + shadow map:
    vec4 currPixPos = (uniform_Mshadow * vec4(varying_vert*bar, 1.0));  // Now in light space
    currPixPos = currPixPos / currPixPos.w;
    float shadow;
    float xx = currPixPos.x;
    float yy = currPixPos.y;
    if (xx >= width || yy >= height || xx < 0 || yy < 0)
        shadow = 1;
    else {
        int idx = int(xx) + int(yy*width);
        shadow = 0.3 + 0.7*(currPixPos.z < (shadowmap[idx]));
        TGAColor c(255, 255, 255);
        if (currPixPos.z < (shadowmap[idx] - 0.01)) {
            img.set(xx, height-yy, c * (1 - currPixPos.z));
            gl_Fragcolor = vec3(255, 0, 0);
            return false;
        }
    }
    shadow = 1;

    mat<3,3> v = varying_vert.transpose(); // For better indexing
    vec3 bn = (varying_norm*bar).normalize();
    vec3 du = varying_uv[0] - varying_uv[0][0];
    vec3 dv = varying_uv[1] - varying_uv[1][0];
    vec3 t = (dv[2]*(v[1] - v[0]) - dv[1]*(v[2] - v[0])) / (du[1]*dv[2] - du[2]*dv[1]); // Compute tangent
    t = (t - (t*bn)*bn).normalize();    // Schmidt orthogonalize tangent
    vec3 b = cross(bn, t);       // Bitangent      
    mat3 TBN = {t, b, bn};
    TBN = TBN.transpose();

    vec3 n = (TBN * model->normal_tangent(uv)).normalize();
    vec3 l = -uniform_light;
    vec3 r = (n*(n*l*2.f) - l).normalize();
    float ambi = 10; // Ambient term
    float diff = std::max(n*l, 0.0);    // diffuse term
    float spec = 0.5*pow(std::max(r.z, 0.0), model->specular(uv));  // Specular term, r.z = cos(theta), theta is the angle between z axis and r.
    float intensity = diff + spec;

    TGAColor c = model->diffuse(uv);
    for (int i = 0; i < 3; i ++) gl_Fragcolor[i] = std::min<float>(ambi + c[i]*intensity*shadow, 255);
    // gl_Fragcolor = std::min<TGAColor>(c*intensity*shadow, {255, 255, 255});

    return false;
}

/***********************************************************************************
********                          Depth Shader                              ********
***********************************************************************************/
vec4 DepthShader::vertex(const int iface, const int nthvert) {
    vec4 gl_vertex, v;
    v = MV * vec4(model->vert(iface, nthvert), 1.0);  // Now in clip space
    gl_vertex = PROJECTION * v;
    varying_vert.setCol(nthvert, gl_vertex.xyz() / gl_vertex.w);
    return v; // Now in screen space
}

bool DepthShader::fragment(const vec3 bar, TGAColor &gl_Fragcolor) {
    double depth = 1 + (varying_vert * bar).z;
    // std::cout << -10*std::log10(depth) << '\n';
    gl_Fragcolor = TGAColor(255, 255, 255) * std::min(-10*std::log10(depth)/6, 1.0);
    return false;
}
