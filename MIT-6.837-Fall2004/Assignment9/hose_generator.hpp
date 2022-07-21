#ifndef hose_generator_hpp
#define hose_generator_hpp

#include <stdio.h>

#include "generator.hpp"

class HoseGenerator: public Generator {
private:
    Vec3f position;
public:
    HoseGenerator(Vec3f _position, float _position_randomness, Vec3f _velocity, float _velocity_randomness): Generator(_position_randomness, _velocity, _velocity_randomness), position(_position) {}
    
    int numNewParticles(float current_time, float dt) const;
    Particle* Generate(float current_time, int i);
    void Paint() const;
};

#endif /* hose_generator_hpp */
