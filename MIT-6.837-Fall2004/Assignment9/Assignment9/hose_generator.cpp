#include "hose_generator.hpp"

int HoseGenerator::numNewParticles(float current_time, float dt) const {
    int num = dt * desired_num_particles / lifespan;
    if (num < 1) {
        return 1;
    }
    return num;
}

Particle* HoseGenerator::Generate(float current_time, int i) {
    Vec3f p = position + position_randomness * r->randomVector();
    Vec3f v = velocity + velocity_randomness * r->randomVector();
    Vec3f c = color + color_randomness * r->randomVector();
    Vec3f dead_c = dead_color + color_randomness * r->randomVector();
    float m = mass + mass_randomness * r->next();
    float l = lifespan + lifespan_randomness * r->next();
    Particle *particle = new Particle(p, v, c, dead_c, m, l);
    return particle;
}

void HoseGenerator::Paint() const {
    
}
