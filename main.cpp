#include <stdlib.h>
#include <math.h>
#include "tgaimage.h"

#define TRUE 1
#define FALSE 0

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void line(int x0, int y0, int x1, int y1, TGAImage *image, TGAColor color)
{
    int steep = FALSE;

    if(abs(x0 - x1) < abs(y0 - y1)) {
        swap(&x0, &y0);
        swap(&x1, &y1);
        steep = TRUE;
    }

    if(x0 > x1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }
    
    int dx = x1 - x0;
    int dy = y1 - y0;
    float derror = fabsf(dy / dx);
    float error = 0;
    int y = y0;

    for(int x = x0; x < x1; x++) {
        float t = (x - x0) / (float)(x1 - x0);
        int y = y0 * (1. - t) + y1 * t;
        if(steep == TRUE) {
            image->set(y, x, color);
        } else {
            image->set(x, y, color);
        }
        error += derror;

        if(error > 0.5) {
            y += (y1 > y0 ? 1: -1);
            error -= 1.;
        }
    }
}

int main(int argc, char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);

    line(13, 20, 80, 40, &image, white);
    line(80, 40, 13, 20, &image, green);
    line(20, 13, 40, 80, &image, red);

    image.flip_vertically();
    image.write_tga_file("out.tga");
    return 0;
}

