#include <iostream>
#include "TGAImage.h"
#include "Model.h"
#include "Camera.h"
#include "GL.h"
#include "Renderer.h"
#include <chrono>

// This porgram use left-hand coordination

const int width = 1024, height = 1024;
const double fov = 45, aspectRatio = 1;
const double near = -0.5, far = -100;
vec3 camPos = 1.2*vec3(1, 1, 3);
vec3 up(0, 1, 0);
vec3 center(0, 0, 0);
vec3 lightPos = vec3(0, 2, -2);
// vec3 lightPos = vec3(2, 2, 2);
vec3 lightdir = (center - lightPos).normalize();
// vec3 camPos = lightPos;

void testShadowMap() {
    TGAImage shadowmap_img(width, height, TGAImage::RGB);
    TGAImage output_img(width, height, TGAImage::RGB);

    Model African_head("E:/Doc/ProjectFiles/CPP/MyRenderer/obj/african_head/african_head.obj");
    Model African_head_eye_inner("E:/Doc/ProjectFiles/CPP/MyRenderer/obj/african_head/african_head_eye_inner.obj");
    std::vector<Model> african_head;
    african_head.push_back(African_head);
    african_head.push_back(African_head_eye_inner);

    // Model Diablo3_pose("E:/Doc/ProjectFiles/CPP/MyRenderer/obj/diablo3_pose/diablo3_pose.obj");
    // std::vector<Model> diablo;
    // diablo.push_back(Diablo3_pose);

    // Model boggie_body("E:/Doc/ProjectFiles/CPP/MyRenderer/obj/boggie/body.obj");
    // Model boggie_eyes("E:/Doc/ProjectFiles/CPP/MyRenderer/obj/boggie/eyes.obj");
    // Model boggie_head("E:/Doc/ProjectFiles/CPP/MyRenderer/obj/boggie/head.obj");
    // std::vector<Model> boggie;
    // boggie.push_back(boggie_body);
    // boggie.push_back(boggie_eyes);
    // boggie.push_back(boggie_head);

    Model *floor = new Model("E:/Doc/ProjectFiles/CPP/MyRenderer/obj/floor/floor.obj");

    std::vector<Model> model2draw;

    /***************************************************************
    *                   First pass: render shadow map              *
    ***************************************************************/
    Renderer renderer;
    renderer.setCamera(camPos, center-camPos, up, width, height, fov, aspectRatio, near, far);

    DepthShader depthshader;
    depthshader.MODEL = mat4::identity() * scale(1);
    depthshader.VIEW = lookAt(lightPos, lightdir, up);
    depthshader.MV = depthshader.VIEW * depthshader.MODEL;
    depthshader.MV_IT = depthshader.MV.invertTranspose();
    depthshader.PROJECTION = projection(renderer.cam().fov, renderer.cam().aspectRatio, renderer.cam().near, renderer.cam().far);
    depthshader.VIEWPORT = viewport(renderer.cam().screen_w, renderer.cam().screen_h);

    model2draw = african_head;
    for (auto model: model2draw) {
        const Model *m = &model;
        depthshader.setModel(m);
        renderer.draw(m, depthshader, shadowmap_img);
    }

    depthshader.setModel(floor);
    renderer.draw(floor, depthshader, shadowmap_img);

    double* shadowmap = renderer.zbuf();
    // for (int i = 0; i < width * height; i ++) std::cout << shadowmap[i] << '\n';

    shadowmap_img.write_tga_file("output_shadowmap.tga");


    renderer.initZbuffer();

    /***************************************************************
    *         Second pass: render object with shadow map           *
    ***************************************************************/
    Shader shader;
    shader.MODEL = mat4::identity() * translate(vec3(0, 0, 0)) * scale(1);
    shader.VIEW = lookAt(renderer.cam().position.xyz(), renderer.cam().direction.xyz(), renderer.cam().up.xyz());
    shader.MV = shader.VIEW * shader.MODEL;
    shader.MV_IT = shader.MV.invertTranspose();
    shader.PROJECTION = projection(renderer.cam().fov, renderer.cam().aspectRatio, renderer.cam().near, renderer.cam().far);
    shader.VIEWPORT = viewport(renderer.cam().screen_w, renderer.cam().screen_h);
    shader.uniform_Mshadow =  depthshader.VIEWPORT * depthshader.PROJECTION * depthshader.MV * (shader.MV).invert();
    shader.uniform_light = (shader.VIEW * vec4(lightdir)).xyz().normalize();
    shader.shadowmap = shadowmap;
    shader.width = width;
    shader.height = height;

    for (auto model: model2draw) {
        const Model *m = &model;
        shader.setModel(m);
        renderer.draw(m, shader, output_img);
    }

    shader.setModel(floor);
    renderer.draw(floor, shader, output_img);

    output_img.write_tga_file("output.tga");

    /***************************************************************
    *                           Debug                              *
    ***************************************************************/
    shader.img.write_tga_file("debug.tga");
}

int main(int argc, char** argv) {
    auto start_time = std::chrono::high_resolution_clock::now();

    testShadowMap();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Time cost: " << duration.count() << " ms" << std::endl;
    return 0;
}