#include <iostream>
#include <assert.h>
using namespace std;

#include "scene_parser.hpp"
#include "camera.hpp"
#include "image.hpp"
#include "group.hpp"
#include "vectors.hpp"
#include "light.hpp"
#include "glCanvas.hpp"
#include "extern.hpp"
#include "ray_tracer.hpp"

char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char *depth_file = NULL;
char *normal_file = NULL;
bool gui = false;
bool shadows = false;
bool transparent_shadows = false;
int max_bounces = 0;
float cutoff_weight = 0;
SceneParser *sp = nullptr;

bool shade_back = false;
int theta_steps = 200;
int phi_steps = 100;
bool gouraud = false;

void parseCommand(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc);
            input_file = argv[i];
        } else if (!strcmp(argv[i],"-size")) {
            i++; assert (i < argc);
            width = atoi(argv[i]);
            i++; assert (i < argc);
            height = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-output")) {
            i++; assert (i < argc);
            output_file = argv[i];
        } else if (!strcmp(argv[i],"-depth")) {
            i++; assert (i < argc);
            depth_min = atof(argv[i]);
            i++; assert (i < argc);
            depth_max = atof(argv[i]);
            i++; assert (i < argc);
            depth_file = argv[i];
        } else if (!strcmp(argv[i],"-normals")) {
            i++; assert (i < argc);
            normal_file = argv[i];
        } else if (!strcmp(argv[i],"-shade_back")) {
            shade_back = true;
        } else if (!strcmp(argv[i], "-gui")) {
            gui = true;
        } else if (!strcmp(argv[i], "-tessellation")) {
            i++; assert(i < argc);
            theta_steps = atof(argv[i]);
            i++; assert(i < argc);
            phi_steps = atof(argv[i]);
        } else if (!strcmp(argv[i], "-gouraud")) {
            gouraud = true;
        } else if (!strcmp(argv[i], "-shadows")) {
            shadows = true;
        } else if (!strcmp(argv[i], "-transparent_shadows")) {
            transparent_shadows = true;
        } else if (!strcmp(argv[i], "-bounces")) {
            i++; assert(i < argc);
            max_bounces = atof(argv[i]);
        } else if (!strcmp(argv[i], "-weight")) {
            i++; assert(i < argc);
            cutoff_weight = atof(argv[i]);
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }
}

void render() {
    Image output_img(width, height);
    Image depth_img(width, height);
    Image normals_img(width, height);
    output_img.SetAllPixels(sp->getBackgroundColor());
    depth_img.SetAllPixels(sp->getBackgroundColor());
    
    Camera *camera = sp->getCamera();
    
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Vec2f point((float)(i+0.5)/width, (float)(j+0.5)/height);
            Ray ray = camera->generateRay(point);
            Hit hit;
            RayTracer rt(sp, max_bounces, cutoff_weight, shadows, transparent_shadows);
            Vec3f color = rt.traceRay(ray, camera->getTMin(), 0, cutoff_weight, 1.0, hit);
            
            output_img.SetPixel(i, j, color);
            
            float depth = hit.getT();
            depth = min(max(depth, depth_min), depth_max);
            float t = (depth_max - depth) / (depth_max - depth_min);
            depth_img.SetPixel(i, j, Vec3f(t, t, t));
            
            Vec3f normal = hit.getNormal();
            normal = (ray.getDirection().Dot3(normal) > 0) && shade_back ? normal * (-1.0f) : normal;
            normals_img.SetPixel(i, j,
                                 Vec3f(abs(normal.x()), abs(normal.y()), abs(normal.z())));
        }
    }
    
    if (output_file) {
        output_img.SaveTGA(output_file);
    }
    if (depth_file) {
        depth_img.SaveTGA(depth_file);
    }
    if (normal_file) {
        normals_img.SaveTGA(normal_file);
    }
}

void traceRay(float x, float y) {
    Camera *camera = sp->getCamera();
    RayTracer rt(sp, max_bounces, cutoff_weight, shadows, transparent_shadows);
    Vec2f point(x, y);
    Ray ray = camera->generateRay(point);
    Hit hit;
    Vec3f color = rt.traceRay(ray, camera->getTMin(), 0, cutoff_weight, 1.0, hit);
}

int main(int argc, char * argv[]) {

    parseCommand(argc, argv);
    sp = new SceneParser(input_file);

    if (gui) {
        GLCanvas canvas;
        canvas.initialize(sp, render, traceRay);
    }
    else {
        render();
    }
    
    return 0;
}
