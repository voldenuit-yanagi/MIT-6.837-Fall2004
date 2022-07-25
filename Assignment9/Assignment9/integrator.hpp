#ifndef integrator_hpp
#define integrator_hpp

#include <stdio.h>

#include "particle.hpp"
#include "forcefield.hpp"

class Integrator {
private:
    //
public:
    Integrator() {}
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;
    virtual Vec3f getColor() = 0;
};

#endif /* integrator_hpp */
