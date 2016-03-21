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

const int width = 200;
const int height = 200;

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

void triangle(Vec2i *pts, TGAImage *image, TGAColor color)
{
    Vec2i bboxmin(image->get_width() - 1, image->get_height() - 1);
    Vec2i bboxmax(0, 0);
    Vec2i clamp(image->get_width() - 1, image->get_height() - 1);

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 2; j++) {
            bboxmin[j] = max(0, min(bboxmin[j], pts[i][j]));
            bboxmax[j] = min(clamp[j], max(bboxmin[j], pts[i][j]));
        }
    }

    Vec2i P;

    for(P.x = bboxmin.x; P.x<=bboxmax.x; P.x++) {
        for(P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
            Vec3f bc_screen = barycentric(pts, P);
            if(bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
            image->set(P.x, P.y, color);
        }
    }

}

int main(int argc, char** argv) {
    TGAImage frame(width, height, TGAImage::RGB);
   
    Vec2i pts[3] = {Vec2i(10, 10), Vec2i(100, 30), Vec2i(190, 160)}; 

    triangle(pts, &frame, TGAColor(255, 0, 0));

    frame.flip_vertically();
    frame.write_tga_file("out.tga");
    return 0;
}
