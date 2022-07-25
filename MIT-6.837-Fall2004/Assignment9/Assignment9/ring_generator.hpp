#ifndef ring_generator_hpp
#define ring_generator_hpp

#include <stdio.h>

#include "generator.hpp"

class RingGenerator: public Generator {
private:
    //
public:
    RingGenerator(float _position_randomness, Vec3f _velocity, float _velocity_randomness): Generator(_position_randomness, _velocity, _velocity_randomness) {}
    
    int numNewParticles(float current_time, float dt) const;
    Particle* Generate(float current_time, int i);
    void Paint() const;
};

#endif /* ring_generator_hpp */
