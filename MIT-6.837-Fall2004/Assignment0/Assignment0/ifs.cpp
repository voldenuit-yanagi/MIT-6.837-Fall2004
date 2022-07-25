#include <stdio.h>
#include <iostream>
#include <time.h>
#include "ifs.hpp"
using namespace std;

void IFS::readDescription(int i, Matrix t, float p) {
    transformations[i] = t;
    probabilities[i] = p;
    if (i == 0) {
        accumulation[i] = p;
    }
    else {
        accumulation[i] = accumulation[i-1] + p;
    }
}

void IFS::render(Image& image, int pointNumber, int iterationNumber) {
    srand((unsigned)time(NULL));
    for (int i=0; i<pointNumber; i++) {
        Vec3f point((float)(rand() % RAND_MAX) / RAND_MAX, (float)(rand() % RAND_MAX) / RAND_MAX, 1);
//        printf("point(%f, %f)\n", point.x(), point.y());
        for (int j=0; j<iterationNumber; j++) {
            float r = (float)rand() / RAND_MAX;
            Matrix transform;
            for (int k=0; k<n; k++) {
                if (r <= accumulation[k]) {
                    transform = transformations[k];
                    break;
                }
            }
            transform.Transform(point);
        }
        point.Scale(image.Width(), image.Height(), 1);
//        printf("new_point(%d, %d)\n", (int)point.x(), (int)point.y());
        Vec3f color(0, 0, 0);
        image.SetPixel((int)point.x(), (int)point.y(), color);
    }
}
