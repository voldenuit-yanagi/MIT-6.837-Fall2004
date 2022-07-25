#include "ray_tracer.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"
#include "extern.hpp"
#include "rayTree.hpp"
#include "raytracing_stats.hpp"

const float EPSILON = 0.001;

void calculateMask(float distTraveled, const Hit &shadowHit, Vec3f &mask) {
    Vec3f mask_temp = Vec3f(1, 1, 1)*(1 - distTraveled) + shadowHit.getMaterial()->getTransparentColor()*distTraveled;
    Vec3f::Mult(mask, mask, mask_temp);
}

// don't need to differentiate RayCast and RayCastFast
// speed up based on the implementation of objects->intersect())
Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight,
                          float indexOfRefraction, Hit &hit) const {
    if (bounces > max_bounces or weight < cutoff_weight) {
        return Vec3f(0, 0, 0);
    }
    
    Object3D *objects;
    if (grid) {
        objects = grid;
    }
    else {
        objects = items;
    }
    
    Vec3f color;
    RayTracingStats::IncrementNumNonShadowRays();
    if (objects -> intersect(ray, hit, tmin)) {
        if (bounces == 0) {
            RayTree::SetMainSegment(ray, 0, hit.getT());
        }
        
        Vec3f p = hit.getIntersectionPoint();
        Material *hitMaterial = hit.getMaterial();
        color = sp->getAmbientLight() * hitMaterial->getDiffuseColor(p);
        
        for (int i=0; i<sp->getNumLights(); i++) {
            Light *light = sp->getLight(i);
            Vec3f directionToLight, lightColor;
            float distanceToLight;
            light -> getIllumination(p, directionToLight, lightColor, distanceToLight);
            Vec3f mask(1.0, 1.0, 1.0);
            if (shadows) {
                Ray shadowRay(p, directionToLight);
                Hit shadowHit(distanceToLight, NULL, Vec3f());
                RayTracingStats::IncrementNumShadowRays();
                if (objects -> intersect(shadowRay, shadowHit, EPSILON)) {
                    if (transparent_shadows && shadowHit.getMaterial()->getTransparentColor().Length() > 0) {
                        float t1, t2, newStart;
                        Vec3f t1TransparentColor;
                        
                        if (shadowRay.getDirection().Dot3(shadowHit.getNormal()) > 0) {
                            t2 = shadowHit.getT();
                            t1 = 0;
                            calculateMask(t2-t1, shadowHit, mask);
                            newStart = t2;
                            shadowHit.set(distanceToLight, NULL, Vec3f(), shadowRay);
                        }
                        else {
                            t1 = shadowHit.getT();
                            t2 = -1;
                            newStart = t1;
                            t1TransparentColor = shadowHit.getMaterial()->getTransparentColor();
                            shadowHit.set(distanceToLight, shadowHit.getMaterial(), Vec3f(), shadowRay);
                        }
                        while (objects -> intersect(shadowRay, shadowHit, newStart+EPSILON)) {
                            Vec3f hitTransparentColor = shadowHit.getMaterial()->getTransparentColor();
                            // not proper way to tell the same object using transparentColor
                            if (t1 > t2 && hitTransparentColor == t1TransparentColor) {
                                t2 = shadowHit.getT();
                                calculateMask(t2-t1, shadowHit, mask);
                                newStart = t2;
                                shadowHit.set(distanceToLight, NULL, Vec3f(), shadowRay);
                            }
                            else {
                                if (hitTransparentColor.Length() > 0) {
                                    t1 = shadowHit.getT();
                                    newStart = t1;
                                    t1TransparentColor = shadowHit.getMaterial()->getTransparentColor();
                                    shadowHit.set(distanceToLight, shadowHit.getMaterial(), Vec3f(), shadowRay);
                                }
                                else {
                                    break;
                                }
                            }
                        }
//                        // ignore the situation that the light in inside the transparent object
//                        if (t1 > t2 && t2 != -1 && ) {
//                            t2 = distanceToLight;
//                            calculateMask(t2-t1, shadowHit, mask);
//                        }
                    }
                    else {
                        //mask = Vec3f(0, 0, 0);
                        RayTree::AddShadowSegment(shadowRay, 0, shadowHit.getT());
                        continue;
                    }
                }
                RayTree::AddShadowSegment(shadowRay, 0, shadowHit.getT());
            }
            color += hitMaterial->Shade(ray, hit, directionToLight, lightColor) * mask;
        }
        
        if (bounces < max_bounces) {
            Vec3f normal = hit.getNormal();
            
            Vec3f reflectiveColor = hitMaterial->getReflectiveColor(p);
            if (reflectiveColor.Length() > 0) {
                Ray reflectiveRay(p, mirrorDirection(normal, ray.getDirection()));
                Hit reflectiveHit;
                color += traceRay(reflectiveRay, EPSILON, bounces+1, weight*reflectiveColor.Length(), hitMaterial->getIndexOfRefraction(p), reflectiveHit) * reflectiveColor;
                RayTree::AddReflectedSegment(reflectiveRay, 0, reflectiveHit.getT());
            }
            
            Vec3f transparentColor = hitMaterial->getTransparentColor(p);
            if (transparentColor.Length() > 0) {
                Vec3f transmittedDir;
                bool refraction = false;
                bool inside = ray.getDirection().Dot3(normal) > 0;
                if (shade_back && inside) {
                    normal = normal * (-1.0f);
                    refraction = transmittedDirection(normal, ray.getDirection(), indexOfRefraction, 1.0, transmittedDir);
                }
                else {
                    refraction = transmittedDirection(normal, ray.getDirection(), 1.0, hitMaterial->getIndexOfRefraction(p), transmittedDir);
                }
                
                if (refraction) {
                    Ray transmittedRay(p, transmittedDir);
                    Hit transmittedHit;
                    if (shade_back && inside) {
                        color += traceRay(transmittedRay, EPSILON, bounces + 1, weight*transparentColor.Length(), 1.0, transmittedHit) * transparentColor;
                    }
                    else {
                        color += traceRay(transmittedRay, EPSILON, bounces + 1, weight*transparentColor.Length(), hitMaterial->getIndexOfRefraction(p), transmittedHit) * transparentColor;
                    }
                    RayTree::AddTransmittedSegment(transmittedRay, 0, transmittedHit.getT());
                }
            }
        }
    }
    else {
        if (bounces == 0) {
            RayTree::SetMainSegment(ray, 0, std::numeric_limits<float>::max());
        }
        color = sp->getBackgroundColor();
    }
    
    return color;
}

Vec3f RayTracer::mirrorDirection(const Vec3f &normal, const Vec3f &incoming) const{
    Vec3f r = incoming - 2 * (incoming.Dot3(normal)) * normal;
    return r;
}

bool RayTracer::transmittedDirection(const Vec3f &normal, const Vec3f &incoming,
                                     float index_i, float index_t, Vec3f &transmitted) const {
    float nr = index_i / index_t;
    float cosi = normal.Dot3(incoming * (-1.0f));
    float cost2 = 1 - pow(nr, 2) * (1 - pow(cosi, 2));
    if (cost2 < 0) {
        return false;
    }
    transmitted = (nr * cosi - sqrt(cost2)) * normal + nr * incoming;
    transmitted.Normalize();
    return true;
}

