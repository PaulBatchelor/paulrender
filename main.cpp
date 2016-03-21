#include <vector>
#include <stdlib.h>
#include <math.h>
#include "model.h"
#include "tgaimage.h"

#define max(a,b) ((a > b) ? a : b)
#define min(a,b) ((a < b) ? a : b)

#define TRUE 1
#define FALSE 0

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

const int width = 800;
const int height = 800;

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_vec2(Vec2i *a, Vec2i *b)
{
    Vec2i tmp = *a;
    *a = *b;
    *b = tmp;
}

void line(Vec2i p0, Vec2i p1, TGAImage *image, TGAColor color)
{
    int steep = FALSE;

    if(abs(p0.x - p1.x) < abs(p0.y - p1.y)) {
        swap(&p0.x, &p0.y);
        swap(&p1.x, &p1.y);
        steep = TRUE;
    }

    if(p0.x > p1.x) {
        swap_vec2(&p0, &p1);
    }
    
    for(int x = p0.x; x < p1.x; x++) {
        float t = (x - p0.x) / (float)(p1.x - p0.x);
        int y = p0.y * (1. - t) + p1.y * t;

        if(steep == TRUE) {
            image->set(y, x, color);
        } else {
            image->set(x, y, color);
        }
    }
}

Vec3f barycentric(Vec2i *pts, Vec2i P) {
    Vec3f u = cross( 
        Vec3f(pts[2][0] - pts[0][0],
        pts[1][0] - pts[0][0],
        pts[0][0] - P[0]),
        Vec3f(pts[2][1] - pts[0][1],
        pts[1][1] - pts[0][1],
        pts[0][1] - P[1]));
    if(abs(u[2]) < 1) return Vec3f(-1, 1, 1);
    return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage *image, TGAColor color)
{
    if(t0.y == t1.y && t0.y == t2.y) return;

    if(t0.y > t1.y) swap_vec2(&t0, &t1);
    if(t0.y > t2.y) swap_vec2(&t0, &t2);
    if(t1.y > t2.y) swap_vec2(&t1, &t2);

    int total_height = t2.y - t0.y; 

    for(int i = 0; i < total_height; i++) {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
        if(A.x > B.x) swap_vec2(&A, &B);
        for(int j = A.x; j<=B.x; j++) {
            image->set(j, t0.y + i, color);
        }
    }
}

int main(int argc, char** argv) {

    Model *model = new Model("obj/african_head.obj");
    TGAImage frame(width, height, TGAImage::RGB);
  
    for(int i=0; i< model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        for(int j=0; j < 3; j++) {
            Vec3f world_coords = model->vert(face[j]);
            screen_coords[j] = Vec2i((world_coords.x + 1.) * width / 2., 
                (world_coords.y + 1.) * height / 2.); 
        }
        triangle(screen_coords[0], screen_coords[1], screen_coords[2], 
            &frame, TGAColor(rand() % 255, rand() % 255, rand() % 255));

    }
    frame.flip_vertically();
    frame.write_tga_file("out.tga");
    return 0;
}
