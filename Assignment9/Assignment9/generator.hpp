#ifndef generator_hpp
#define generator_hpp

#include <stdio.h>

#include "particle.hpp"
#include "vectors.hpp"
#include "random.hpp"

class Generator {
protected:
    Random *r;
    float position_randomness = 0;
    Vec3f velocity;
    float velocity_randomness = 0;
    Vec3f color;
    Vec3f dead_color;
    float color_randomness = 0;
    float lifespan;
    float lifespan_randomness = 0;
    int desired_num_particles;
    float mass;
    float mass_randomness = 0;
public:
    Generator(float _position_randomness, Vec3f _velocity, float _velocity_randomness): position_randomness(_position_randomness), velocity(_velocity), velocity_randomness(_velocity_randomness) { r = new Random(); }
    
    // initialization
    void SetColors(Vec3f _color, Vec3f _dead_color, float _color_randomness) {
        color = _color;
        dead_color = _dead_color;
        color_randomness = _color_randomness;
    }
    void SetLifespan(float _lifespan, float _lifespan_randomness, int _desired_num_particles) {
        lifespan = _lifespan;
        lifespan_randomness = _lifespan_randomness;
        desired_num_particles = _desired_num_particles;
    }
    void SetMass(float _mass, float _mass_randomness) {
        mass = _mass;
        mass_randomness = _mass_randomness;
    }

    // on each timestep, create some particles
    virtual int numNewParticles(float current_time, float dt) const = 0;
    virtual Particle* Generate(float current_time, int i) = 0;

    // for the gui
    virtual void Paint() const = 0;
    void Restart() { delete r; r = new Random(); }
};

#endif /* generator_hpp */
