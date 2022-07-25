#include "ring_generator.hpp"

int RingGenerator::numNewParticles(float current_time, float dt) const {
    return (current_time>3 ? 3 : current_time) * dt * desired_num_particles / lifespan;
}

Particle* RingGenerator::Generate(float current_time, int i) {
    float radius = current_time > 3 ? 3 : current_time;
    
    Vec3f p = r->randomVector();
    p = Vec3f(p.x(), 0, p.y());
    p.Normalize();
    p *= radius;
    Vec3f random_p = r->randomVector();
    random_p = Vec3f(random_p.x(), 0, random_p.y());
    random_p.Normalize();
    random_p *= position_randomness;
    p.Set(p.x() + random_p.x(), -4, p.z()+random_p.z());
    
    Vec3f v = velocity + velocity_randomness * r->randomVector();
    Vec3f c = color + color_randomness * r->randomVector();
    Vec3f dead_c = dead_color + color_randomness * r->randomVector();
    float m = mass + mass_randomness * r->next();
    float l = lifespan + lifespan_randomness * r->next();
    Particle *particle = new Particle(p, v, c, dead_c, m, l);
    return particle;
}

void RingGenerator::Paint() const {
    
}
