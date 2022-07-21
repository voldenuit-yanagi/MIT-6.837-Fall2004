#ifndef midpoint_integrator_hpp
#define midpoint_integrator_hpp

#include <stdio.h>

#include "integrator.hpp"

class MidpointIntegrator: public Integrator {
private:
    //
public:
    MidpointIntegrator() {}
    
    void Update(Particle *particle, ForceField *forcefield, float t, float dt) {
        particle->increaseAge(dt);
        Vec3f p = particle->getPosition();
        Vec3f v = particle->getVelocity();
        Vec3f pm = p + v * dt * 0.5;
        Vec3f vm = v + forcefield->getAcceleration(p, particle->getMass(), t) * dt * 0.5;
        particle->setPosition(p + vm * dt);
        particle->setVelocity(v + forcefield->getAcceleration(pm, particle->getMass(), t+0.5*dt) * dt);
    }
    
    Vec3f getColor() {
        return Vec3f(170, 199, 207) * (1.f/256);
    }
};

#endif /* midpoint_integrator_hpp */
