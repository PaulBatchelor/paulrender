#include <vector>
#include <stdlib.h>
#include <math.h>
#include "model.h"
#include "tgaimage.h"

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

//void line(int x0, int y0, int x1, int y1, TGAImage *image, TGAColor color)
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

void render_head()
{
    //Model *model = new Model("obj/african_head.obj");

    //TGAImage image(width, height, TGAImage::RGB);
   
    //for(int i = 0; i < model->nfaces(); i++) {
    //    std::vector<int> face = model->face(i);
    //    for(int j = 0; j < 3; j++) {
    //        Vec3f v0 = model->vert(face[j]);
    //        Vec3f v1 = model->vert(face[(j + 1) % 3]);
    //        int x0 = (v0.x + 1.) * width / 2;
    //        int y0 = (v0.y + 1.) * height / 2;
    //        int x1 = (v1.x + 1.) * width / 2;
    //        int y1 = (v1.y + 1.) * height / 2;
    //        line(x0, y0, x1, y1, &image, white);
    //    }
    //}
 
    //image.flip_vertically();
    //image.write_tga_file("out.tga");
    //delete model;
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage *image, TGAColor color)
{
    line(t0, t1, image, color);
    line(t1, t2, image, color);
    line(t2, t0, image, color);
}

int main(int argc, char** argv) {
    //render_head();
    TGAImage image(width, height, TGAImage::RGB);
    
    Vec2i t0[3] = {
        Vec2i(10, 70),
        Vec2i(50, 160),
        Vec2i(70, 80)
    };

    Vec2i t1[3] = {
        Vec2i(180, 50),
        Vec2i(150, 1),
        Vec2i(70, 180)
    };

    Vec2i t2[3] = {
        Vec2i(180, 150),
        Vec2i(120, 160),
        Vec2i(130, 180)
    };

    triangle(t0[0], t0[1], t0[2], &image, red);
    triangle(t1[0], t1[1], t1[2], &image, white);
    triangle(t2[0], t2[1], t2[2], &image, green);

    image.flip_vertically();
    image.write_tga_file("out.tga");
    return 0;
}
