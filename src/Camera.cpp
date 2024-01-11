#include "Camera.h"

Camera::Camera() {
    position = vec4(0.f, 0.f, 0.f, 1.f);
    direction = vec4(0.f, 0.f, -1.f, 0.f);
    up = vec4(0.f, 1.f, 0.f, 0.f);
    right = vec4(cross(direction.xyz(), up.xyz()).normalize(), 0.f);
    up = vec4(cross(right.xyz(), direction.xyz()).normalize(), 0.f);
    fov = 45;
    aspectRatio = 1;
    near = -0.5;
    far = -50;
    height = -2 * near * tan(fov*PI/180/2);
    width = height * aspectRatio;
    screen_w = 100;
    screen_h = 100;
    frame = TGAImage(screen_w, screen_h, TGAImage::RGB);
    zbuffer = TGAImage(screen_w, screen_h, TGAImage::RGB);
}

Camera::Camera(vec3 pos, vec3 dir, vec3 u, double w, double h, double fov, double r, double n, double f) {
    position = vec4(pos, 1.f);
    direction = vec4(dir.normalize(), 0.f);
    right = vec4(cross(direction.xyz(), u).normalize(), 0.f);
    up = vec4(cross(right.xyz(), direction.xyz()).normalize(), 0.f);
    this->fov = fov*PI/180.0;
    aspectRatio = r;
    near = n;
    far = f;
    fov = fov;
    height = -2 * near * tan(fov*PI/180/2);
    width = height * aspectRatio;
    screen_w = w;
    screen_h = h;
    frame = TGAImage(screen_w, screen_h, TGAImage::RGB);
    zbuffer = TGAImage(screen_w, screen_h, TGAImage::RGB);
}

void Camera::set(vec3 pos, vec3 dir, vec3 u, double w, double h, double fov, double r, double n, double f) {
    position = vec4(pos, 1.f);
    direction = vec4(dir.normalize(), 0.f);
    right = vec4(cross(direction.xyz(), u).normalize(), 0.f);
    up = vec4(cross(right.xyz(), direction.xyz()).normalize(), 0.f);
    this->fov = fov;
    aspectRatio = r;
    near = n;
    far = f;
    height = -2 * near * tan(fov*PI/180/2);
    width = height * aspectRatio;
    screen_w = w;
    screen_h = h;
    frame = TGAImage(screen_w, screen_h, TGAImage::RGB);
    zbuffer = TGAImage(screen_w, screen_h, TGAImage::RGB);
}

mat4 Camera::lookAt() {
    vec3 z = -direction.xyz().normalize();
    vec3 x = cross(up.xyz(), z).normalize();
    vec3 y = cross(z, x).normalize();

    mat4 lookat = mat4::identity();
    for (int i = 0; i < 3; i ++) {
        lookat[0][i] = x[i];
        lookat[1][i] = y[i];
        lookat[2][i] = z[i];
        lookat[i][3] = -position[i];
    }
    return lookat;
}

mat4 Camera::projection() {
    double w = width;
    double h = height;
    double n = near;
    double f = far;
    mat4 ortho, persp2ortho;
    ortho = {{{2.0/w, 0.f, 0.f, 0.f},
                    {0.f, 2.0/h, 0.f, 0.f},
                    {0.f, 0.f, 2.0/(n-f), (n+f)/(n-f)},
                    {0.f, 0.f, 0.f, 1.f}}};
    persp2ortho = {{{n, 0.f, 0.f, 0.f},
                        {0.f, n, 0.f, 0.f},
                        {0.f, 0.f, n+f, -n*f},
                        {0.f, 0.f, 1.f, 0.f}}};
    return ortho * persp2ortho;
}

mat4 Camera::viewport() {
    mat4 m = mat4::identity();
    m[0][3] = screen_w / 2.f;
    m[1][3] = screen_h / 2.f;
    // m[2][3] = depth / 2.f;

    m[0][0] = screen_w / 2.f;
    m[1][1] = screen_h / 2.f;
    // m[2][2] = depth / 2.f;
    return m;
}