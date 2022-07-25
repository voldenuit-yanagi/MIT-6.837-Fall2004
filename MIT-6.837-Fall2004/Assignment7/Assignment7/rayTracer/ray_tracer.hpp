#ifndef ray_tracer_hpp
#define ray_tracer_hpp

#include <stdio.h>

#include "scene_parser.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include "grid.hpp"
#include "extern.hpp"
#include "group.hpp"

class RayTracer {
private:
    SceneParser *sp;
    int max_bounces;
    float cutoff_weight;
    bool shadows;
    Group *items;
    bool transparent_shadows;
    Grid *grid;
public:
    RayTracer(SceneParser *s, int max_bounces_, float cutoff_weight_, bool shadows_, bool transparent_shadows_, Grid *grid_) {
        sp = s;
        max_bounces = max_bounces_;
        cutoff_weight = cutoff_weight_;
        shadows = shadows_;
        items = sp->getGroup();
        transparent_shadows = transparent_shadows_;
        grid = grid_;
    }
    ~RayTracer() {}
    
    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight,
                   float indexOfRefraction, Hit &hit) const;
    
    Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming) const;
    bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming,
          float index_i, float index_t, Vec3f &transmitted) const;
    
    
};

#endif /* ray_tracer_hpp */
