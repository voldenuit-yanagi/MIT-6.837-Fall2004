#include <iostream>
#include <stdio.h>

#include "scene_parser.hpp"
#include "orthographic_camera.hpp"
#include "image.hpp"
#include "vectors.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include "group.hpp"
#include "material.hpp"

int main(int argc, char * argv[]) {
    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;

    // sample command line:
    // raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga
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
      } else {
        printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
        assert(0);
      }
    }
    
    SceneParser sp(input_file);
    Camera* camera = sp.getCamera();
    Group* items = sp.getGroup();
    
    Image img(width, height);
    img.SetAllPixels(sp.getBackgroundColor());
    Image depth_img(width, height);
    depth_img.SetAllPixels(Vec3f(0, 0, 0));
    
    for (int i=0; i<width; i++) {
        for (int j=0; j<height; j++) {
//    for (int i=100; i<101; i++) {
//        for (int j=100; j<101; j++) {
            Vec2f point((float)(i+0.5)/width, (float)(j+0.5)/height);
            Ray r = camera->generateRay(point);
            Hit h;
            if (items->intersect(r, h, camera->getTMin())) {
                img.SetPixel(i, j, (h.getMaterial())->getDiffuseColor());
                float depth = h.getT();
                depth = min(max(depth, depth_min), depth_max);
                float t = (depth_max - depth) / (depth_max - depth_min);
                depth_img.SetPixel(i, j, Vec3f(t, t, t));
            }
        }
    }
    
    img.SaveTGA(output_file);
    depth_img.SaveTGA(depth_file);
    
    return 0;
}
