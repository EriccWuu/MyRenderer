#include "Renderer.h"

Renderer::Renderer() {
    zbuffer = nullptr;
    camera = Camera();
    frame_img = TGAImage(camera.screen_w, camera.screen_h, TGAImage::RGB);
    zbuffer_img = TGAImage(camera.screen_w, camera.screen_h, TGAImage::RGB);
}

Renderer::~Renderer() {
    if (zbuffer != nullptr) {
        delete zbuffer;
        zbuffer = nullptr;
    }
}

void Renderer::setCamera(vec3 pos, vec3 dir, vec3 up, double w, double h, double fov, double r, double n, double f) {
    camera.set(pos, dir, up, w, h, fov, r, n, f);
    initZbuffer();
    frame_img = TGAImage(camera.screen_w, camera.screen_h, TGAImage::RGB);
    zbuffer_img = TGAImage(camera.screen_w, camera.screen_h, TGAImage::RGB);
}

void Renderer::initZbuffer() {
    int size = camera.screen_w * camera.screen_h;
    if (zbuffer == nullptr) zbuffer = new double[size];
    else { delete zbuffer; zbuffer = new double[size]; }
    for (int i = camera.screen_w*camera.screen_h; i --; zbuffer[i] = -1);
}

Camera& Renderer::cam() {
    return camera;
}

double* Renderer::zbuf() {
    int size = camera.screen_w*camera.screen_h;
    double *zbuf = new double[size];
    std::memcpy(zbuf, zbuffer, sizeof(double) * size);
    return zbuf;
}

// void Renderer::draw(const Model *model, IShader &shader) {
void Renderer::draw(const Model *model, IShader &shader, TGAImage &image, bool DRAW_MESH) {
    if (model == nullptr) {
        std::cerr << "No Model can be draw." << '\n';
        return ;
    }

    for (int i = 0; i < model->nfaces(); i ++) {
        mat<3,4> v;
        for (int j = 0; j < 3; j ++) {
            v[j] = shader.vertex(i, j);  // Vertex Shader Process
        }

        // Back-face culling
        vec3 n = cross((v[1] - v[0]).xyz(), (v[2] - v[0]).xyz());
        if (n.z < 0) continue;

        v = (shader.VIEWPORT * shader.PROJECTION * v.transpose()).transpose();

        Triangle t(v);
        if (DRAW_MESH)  drawTriangle_mesh(t, image, TGAColor(255, 255, 255));
        else            drawTriangle(t, shader, image, zbuffer);
    }

}

void Renderer::draw_zbuffer() {
    int width = camera.screen_w;
    int height = camera.screen_h;
    for (int i = 0; i < width; i ++)
        for (int j = 0; j < height; j ++) {
            // std::cout << zbuffer[i + j*width] << '\n';
            TGAColor c = TGAColor(255, 255, 255) * std::min(-10*std::log10(1 + zbuffer[i + j*width]) / 6, 1.0);
            zbuffer_img.set(i, height-j, c);
        }
}