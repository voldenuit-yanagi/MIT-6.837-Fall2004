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

char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char *depth_file = NULL;
char *normal_file = NULL;
bool shade_back = false;
bool gui = false;
SceneParser *sp = nullptr;

int theta_steps = 0;
int phi_steps = 0;
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
    Group *items = sp->getGroup();
    
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
//    for (int i = 100; i < 101; i++) {
//        for (int j = 0; j < 1; j++) {
            Vec2f point((float)(i+0.5)/width, (float)(j+0.5)/height);
            Ray ray = camera->generateRay(point);
            Hit hit;
            if (items->intersect(ray, hit, camera->getTMin())) {
                Vec3f color = sp->getAmbientLight() * hit.getMaterial()->getDiffuseColor();
                Vec3f normal = hit.getNormal();
                float dot = ray.getDirection().Dot3(normal);
                if (shade_back && dot > 0) {
                    normal = normal * (-1.0f);
                    hit.set(hit.getT(), hit.getMaterial(), normal, ray);
                }
                for (int k=0; k<sp->getNumLights(); k++) {
                    Light *light = sp->getLight(k);
                    Vec3f p = hit.getIntersectionPoint();
                    Vec3f directionToLight, lightColor;
                    float distanceToLight;
                    light -> getIllumination(p, directionToLight, lightColor, distanceToLight);
                    color += hit.getMaterial()->Shade(ray, hit, directionToLight, lightColor);
                }
                
                output_img.SetPixel(i, j, color);
                
                float depth = hit.getT();
                depth = min(max(depth, depth_min), depth_max);
                float t = (depth_max - depth) / (depth_max - depth_min);
                depth_img.SetPixel(i, j, Vec3f(t, t, t));
                
                normals_img.SetPixel(i, j,
                                     Vec3f(abs(normal.x()), abs(normal.y()), abs(normal.z())));
            }
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

int main(int argc, char * argv[]) {

    // -input scene2_05_inside_sphere.txt -size 200 200 -output output2_05.tga -depth 9 11 depth2_05.tga -normals normals2_05.tga -shade_back
    parseCommand(argc, argv);
    sp = new SceneParser(input_file);

    if (gui) {
        GLCanvas canvas;
        canvas.initialize(sp, render);
    }
    else {
        render();
    }
    
    return 0;
}
