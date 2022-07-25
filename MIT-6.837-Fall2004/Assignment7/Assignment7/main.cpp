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
#include "grid.hpp"
#include "raytracing_stats.hpp"
#include "sampler.hpp"
#include "random_sampler.hpp"
#include "uniform_sampler.hpp"
#include "jittered_sampler.hpp"
#include "film.hpp"
#include "filter.hpp"
#include "box_filter.hpp"
#include "tent_filter.hpp"
#include "gaussian_filter.hpp"

char *input_file = NULL;
char *output_file = NULL;
char *sample_file = NULL;
char *filter_file = NULL;
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
Group *items = nullptr;
bool useGrid = false;
Grid *grid = NULL;
int nx = 0, ny = 0, nz = 0;
bool tracer_status = false;
Sampler *sampler = new UniformSampler(1);
int sample_zoom = 0;
int num_samples = 1;
Filter *filter = nullptr;
int filter_zoom = 0;

int width = 100;
int height = 100;
bool shade_back = false;
int theta_steps = 200;
int phi_steps = 100;
bool gouraud = false;
bool visualize_grid = false;

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
            theta_steps = atoi(argv[i]);
            i++; assert(i < argc);
            phi_steps = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-gouraud")) {
            gouraud = true;
        } else if (!strcmp(argv[i], "-shadows")) {
            shadows = true;
        } else if (!strcmp(argv[i], "-transparent_shadows")) {
            transparent_shadows = true;
        } else if (!strcmp(argv[i], "-bounces")) {
            i++; assert(i < argc);
            max_bounces = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-weight")) {
            i++; assert(i < argc);
            cutoff_weight = atof(argv[i]);
        } else if (!strcmp(argv[i], "-grid")) {
            useGrid = true;
            i++; assert(i < argc);
            nx = atoi(argv[i]);
            i++; assert(i < argc);
            ny = atoi(argv[i]);
            i++; assert(i < argc);
            nz = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-visualize_grid")) {
            visualize_grid = true;
        } else if (!strcmp(argv[i], "-stats")) {
            tracer_status = true;
        } else if (!strcmp(argv[i], "-render_samples")) {
            i++; assert(i < argc);
            sample_file = argv[i];
            i++; assert(i < argc);
            sample_zoom = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-random_samples")) {
            i++; assert(i < argc);
            num_samples = atoi(argv[i]);
            sampler = new RandomSampler(num_samples);
        } else if (!strcmp(argv[i], "-uniform_samples")) {
            i++; assert(i < argc);
            num_samples = atoi(argv[i]);
            sampler = new UniformSampler(num_samples);
        } else if (!strcmp(argv[i], "-jittered_samples")) {
            i++; assert(i < argc);
            num_samples = atoi(argv[i]);
            sampler = new JitteredSampler(num_samples);
        } else if (!strcmp(argv[i], "-render_filter")) {
            i++; assert(i < argc);
            filter_file = argv[i];
            i++; assert(i < argc);
            filter_zoom = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-box_filter")) {
            i++; assert(i < argc);
            float radius = atof(argv[i]);
            filter = new BoxFilter(radius);
        } else if (!strcmp(argv[i], "-tent_filter")) {
            i++; assert(i < argc);
            float radius = atof(argv[i]);
            filter = new TentFilter(radius);
        } else if (!strcmp(argv[i], "-gaussian_filter")) {
            i++; assert(i < argc);
            float sigma = atof(argv[i]);
            filter = new GaussianFilter(sigma);
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }
}

void render() {
    Image output_img(width, height);
    output_img.SetAllPixels(sp->getBackgroundColor());
    
    Camera *camera = sp->getCamera();
    RayTracer rt(sp, max_bounces, cutoff_weight, shadows, transparent_shadows, grid);
    Film film(width, height, num_samples);
    
    if (tracer_status) {
        RayTracingStats::Initialize(width, height, items->getBoundingBox(), nx, ny, nz);
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int n=0; n < num_samples; n++) {
                Vec2f offset = sampler->getSamplePosition(n);
                Vec2f point((float)(i+offset.x())/width, (float)(j+offset.y())/height);
                Ray ray = camera->generateRay(point);
                Hit hit;
                Vec3f color = rt.traceRay(ray, camera->getTMin(), 0, 1.0, 1.0, hit);
                film.setSample(i, j, n, offset, color);
            }
        }
    }
    if (tracer_status) {
        RayTracingStats::PrintStatistics();
    }
    
    if (output_file) {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if (filter) {
                    output_img.SetPixel(i, j, filter->getColor(i, j, &film));
                }
                else {
                    output_img.SetPixel(i, j, film.getSample(i, j, 0).getColor());
                }
            }
        }
        output_img.SaveTGA(output_file);
    }
    if (sample_file) {
        film.renderSamples(sample_file, sample_zoom);
    }
    if (filter_file) {
        film.renderFilter(filter_file, filter_zoom, filter);
    }
}

void traceRay(float x, float y) {
    cout << x << " " << y << endl;
    Camera *camera = sp->getCamera();
    RayTracer rt(sp, max_bounces, cutoff_weight, shadows, transparent_shadows, grid);
    Vec2f point(x, y);
    Ray ray = camera->generateRay(point);
    Hit hit;
    rt.traceRay(ray, camera->getTMin(), 0, 1.0, 1.0, hit);
}

int main(int argc, char * argv[]) {
    parseCommand(argc, argv);
    sp = new SceneParser(input_file);
    items = sp->getGroup();
    if (useGrid) {
        grid = new Grid(items->getBoundingBox(), nx, ny, nz);
        items->insertIntoGrid(grid, NULL);
    }

    if (gui) {
        GLCanvas canvas;
        canvas.initialize(sp, render, traceRay, grid, visualize_grid);
    }
    else {
        render();
    }
    
    return 0;
}
