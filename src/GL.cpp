#include "GL.h"

// Model transform
mat4 scale(double s) {
    mat4 S = mat4::identity();
    for (int i = 0; i < 3; i ++) S[i][i] = s;
    return S;
}

mat4 translate(vec3 t) {
    mat4 T = mat4::identity();
    for (int i = 0; i < 3; i ++) T[i][3] = t[i];
    return T;
}

mat4 rotateX(double degree) {
    double radian = degree*PI/180;
    mat4 R;
    R = {{{1.f, 0.f, 0.f, 0.f},
                {0.f, cos(radian), -sin(radian), 0.f},
                {0.f, sin(radian), cos(radian), 0.f},
                {0.f, 0.f, 0.f, 1.f}}};
    return R;
}

mat4 rotateY(double degree) {
    double radian = degree*PI/180;
    mat4 R;
    R = {{{cos(radian), 0.f, sin(radian), 0.f},
                {0.f, 1.f, 0.f, 0.f},
                {-sin(radian), 0.f, cos(radian), 0.f},
                {0.f, 0.f, 0.f, 1.f}}};
    return R;
}

mat4 rotateZ(double degree) {
    double radian = degree*PI/180;
    mat4 R;
    R = {{{ cos(radian), -sin(radian), 0.f, 0.f},
                { sin(radian),  cos(radian), 0.f, 0.f},
                { 0.f, 0.f, 1.f, 0.f},
                { 0.f, 0.f, 0.f, 1.f}}};
    return R;
}

mat4 rotate(vec3 n, double degree) {
    mat4 R = mat4::identity();
    return R;
}

// View transform
mat4 lookAt(vec3 position, vec3 direction, vec3 up) {
    vec3 z = -direction.normalize();
    vec3 x = cross(up, z).normalize();
    vec3 y = cross(z, x).normalize();
    // vec3 x = cross(z, up).normalize();
    // vec3 y = cross(x, z).normalize();
    mat4 Rview = mat4::identity();
    mat4 Tview = mat4::identity();
    Tview[0][3] = -position.x;
    Tview[1][3] = -position.y;
    Tview[2][3] = -position.z;
    Rview[0] = vec4(x);
    Rview[1] = vec4(y);
    Rview[2] = vec4(z);
    return Rview * Tview;
}

// Projection transform
mat4 projection(double fov, double aspectRatio, double near, double far) {
    double h = -2*near*tan(fov*PI/180/2);
    double w = aspectRatio * h;
    double n = near;
    double f = far;
    mat4 ortho, persp2ortho;
    ortho = {{{2.0/w, 0.f, 0.f, 0.f},
                    {0.f, 2.0/h, 0.f, 0.f},
                    {0.f, 0.f, 2.0/(n-f), -(n+f)/(n-f)},
                    {0.f, 0.f, 0.f, 1.f}}};
    persp2ortho = {{{n, 0.f, 0.f, 0.f},
                        {0.f, n, 0.f, 0.f},
                        {0.f, 0.f, n+f, -n*f},
                        {0.f, 0.f, 1.f, 0.f}}};
    return ortho * persp2ortho;
}

// Viewport transform
mat4 viewport(int screen_w, int screen_h) {
    mat4 m = mat4::identity();
    m[0][3] = screen_w / 2.f;
    m[1][3] = screen_h / 2.f;

    m[0][0] = screen_w / 2.f;
    m[1][1] = screen_h / 2.f;
    return m;
}

// Find bounding box of triangle
std::vector<vec2> bounding_box(Triangle t) {
    vec2 bottomleft, upperright;
    bottomleft.x = std::floor(std::min(t.vert[0].x, std::min(t.vert[1].x, t.vert[2].x)));
    bottomleft.y = std::floor(std::min(t.vert[0].y, std::min(t.vert[1].y, t.vert[2].y)));
    upperright.x = std::ceil(std::max(t.vert[0].x, std::max(t.vert[1].x, t.vert[2].x)));
    upperright.y = std::ceil(std::max(t.vert[0].y, std::max(t.vert[1].y, t.vert[2].y)));
    std::vector<vec2> bbox = {bottomleft, upperright};
    return bbox;
}

// Determine if the point is inside triangle
bool isInsideTriangle(double x, double y, Triangle t) {
    vec3 p = {x, y, 1.0};
    // compute 2D barycentric coordinates
    vec3 a = vec3(t.vert[2].x - t.vert[0].x, t.vert[1].x - t.vert[0].x, t.vert[0].x - p.x);
    vec3 b = vec3(t.vert[2].y - t.vert[0].y, t.vert[1].y - t.vert[0].y, t.vert[0].y - p.y);
    vec3 u = cross(a, b);
    vec3 abc;
    if (std::abs(u.z) > 1e-3) abc = vec3(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    else abc = vec3(-1, 1, 1);

    if (abc.x < 0 || abc.y < 0 || abc.z < 0) return false;
    return true;
}

// Compute barycentric coordinate in screen space
static std::tuple<double, double, double> computeBarycentric(double x, double y, const mat<3,4> v) {
    double c1 = (x*(v[1].y - v[2].y) + y*(v[2].x - v[1].x) + v[1].x*v[2].y - v[2].x*v[1].y) / (v[0].x*(v[1].y - v[2].y) + v[0].y*(v[2].x - v[1].x) + v[1].x*v[2].y - v[2].x*v[1].y);
    double c2 = (x*(v[2].y - v[0].y) + y*(v[0].x - v[2].x) + v[2].x*v[0].y - v[0].x*v[2].y) / (v[1].x*(v[2].y - v[0].y) + v[1].y*(v[0].x - v[2].x) + v[2].x*v[0].y - v[0].x*v[2].y);
    double c3 = (x*(v[0].y - v[1].y) + y*(v[1].x - v[0].x) + v[0].x*v[1].y - v[1].x*v[0].y) / (v[2].x*(v[0].y - v[1].y) + v[2].y*(v[1].x - v[0].x) + v[0].x*v[1].y - v[1].x*v[0].y);
    return {c1, c2, c3};
}

// Interpolation with perspective correction
double interpLine(double a1, double a2, double z1, double z2, double s) {
    // z1, z2 is linear depth, s is interpolation factor
    // a1, a2 is attribute to be interpolated
    return ((1-s)*a1*z1 + s*a2*z1) / (s*z1 + (1-s)*z2);
}

// Depth Interpolation with perspective correction
double interpZ(double x, double y, const vec3 bc, double depth[3]) {
    // x, y is the corrdination in screen space
    // bc is the barycentric coordination of point (x, y) 
    // depth is the depth of vertics in view space
    // 1/Z = alpha/Za + beta/Zb + gamma/Zc
    double z_interp = 1.0 / (bc.x / depth[0] + bc.y / depth[1] + bc.z / depth[2]);
    return z_interp;
}

// Attritube Interpolation with perspective correction
double interp(double x, double y, vec3 I, const vec3 bc, double depth[3], double z_interp) {
    // x, y is the corrdination in screen space
    // bc is the barycentric coordination of point (x, y) 
    // depth is the depth of vertics in view space
    // I is the attritube to be interpolated
    double I_interp = bc.x * I[0] / depth[0] + bc.y * I[1] / depth[1] + bc.z * I[2] / depth[2];
    I_interp *= z_interp;
    return I_interp;
}

// draw line
void drawline(int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color) {
    int w = image.width();
    int h = image.height();
    int dx = x2 - x1, dy = y2 - y1;
    bool isSteep = abs(dx) < abs(dy);
    if (isSteep) {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    dx = x2 - x1, dy = y2 - y1;
    bool isNegSlope = dy < 0;

    int eps = 0;
    for (int x = x1, y = y1; x <= x2; x ++) {

        // draw point
        if (!isSteep)
            image.set(x, y, color);
        else
            image.set(y, x, color);

        eps += dy;
        if (!isNegSlope) {
            if ((eps << 1) >= dx) {
                y ++;
                eps -= dx;
            }
        }
        else {
            if ((eps << 1) <= -dx) {
                y --;
                eps += dx;
            }
        }
    }
}

void drawBoundingBox(std::vector<vec2> bbox, TGAImage &image, TGAColor color) {
    drawline(bbox[0].x, bbox[0].y, bbox[1].x, bbox[0].y, image, color);
    drawline(bbox[0].x, bbox[1].y, bbox[1].x, bbox[1].y, image, color);
    drawline(bbox[0].x, bbox[0].y, bbox[0].x, bbox[1].y, image, color);
    drawline(bbox[1].x, bbox[0].y, bbox[1].x, bbox[1].y, image, color);
}

void drawTriangle_mesh(Triangle t,  TGAImage &image, TGAColor color) {
    int height = image.height();
    for (int i = 0; i < 3; i ++) {
        t.vert[i] = t.vert[i] / t.vert[i].w;
    }
    drawline(t.vert[0].x, height - t.vert[0].y, t.vert[1].x, height - t.vert[1].y, image, color);
    drawline(t.vert[1].x, height - t.vert[1].y, t.vert[2].x, height - t.vert[2].y, image, color);
    drawline(t.vert[2].x, height - t.vert[2].y, t.vert[0].x, height - t.vert[0].y, image, color);
}

void drawTriangle(Triangle t, IShader &s, TGAImage &image, double *zbuffer) {
    TGAColor color;
    int w = image.width();
    int h = image.height();

    double depth[3] = {t.vert[0].w, t.vert[1].w, t.vert[2].w};
    vec4 vscreen[3] = {t.vert[0]/depth[0], t.vert[1]/depth[1], t.vert[2]/depth[2]};   // Perspective division
    Triangle tscreen(vscreen);
    std::vector<vec2> bbox = bounding_box(tscreen);

    vec2 p;
    for (p.x = bbox[0].x; p.x <= bbox[1].x; p.x ++) {
        for (p.y = bbox[0].y; p.y <= bbox[1].y; p.y ++) {
            if (p.x >= w || p.y >= h || p.x < 0 || p.y < 0) continue;

            auto [alpha, beta, gamma] = computeBarycentric(p.x, p.y, tscreen.vert);
            if (alpha < 0 || beta < 0 || gamma < 0) continue;   // Determine if the point is inside triangle

            double z = interpZ(p.x, p.y, vec3(alpha, beta, gamma), depth);
            vec3 bc = {alpha*z/depth[0], beta*z/depth[1], gamma*z/depth[2]};
            double z_interp = vec3(tscreen.vert[0].z, tscreen.vert[1].z, tscreen.vert[2].z) * bc;

            if (zbuffer[int(p.x + p.y*w)] > z_interp) continue; // Depth test
            zbuffer[int(p.x + p.y*w)] = z_interp;  // Updata z-buffer

            TGAColor frag_color;
            if (s.fragment(bc, frag_color)) continue; // Fragment shader process
            image.set(p.x, h - p.y, frag_color);
        }
    }
}
