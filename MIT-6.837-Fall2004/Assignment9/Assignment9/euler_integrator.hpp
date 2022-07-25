#ifndef euler_integrator_hpp
#define euler_integrator_hpp

#include <stdio.h>

#include "integrator.hpp"

class EulerIntegrator: public Integrator {
private:
    //
public:
    EulerIntegrator() {}
    
    void Update(Particle *particle, ForceField *forcefield, float t, float dt) {
        particle->increaseAge(dt);
        Vec3f p = particle->getPosition();
        Vec3f v = particle->getVelocity();
        particle->setPosition(p + v * dt);
        particle->setVelocity(v + forcefield->getAcceleration(p, particle->getMass(), t) * dt);
    }
    
    Vec3f getColor() {
        return Vec3f(244, 226, 228) * (1.f/256);
    }
};

#endif /* euler_integrator_hpp */
