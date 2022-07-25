#ifndef rungekutta_integrator_hpp
#define rungekutta_integrator_hpp

#include <stdio.h>

#include "integrator.hpp"

class RungeKuttaIntegrator: public Integrator {
private:
    //
public:
    RungeKuttaIntegrator() {}
    
    void Update(Particle *particle, ForceField *forcefield, float t, float dt) {
        
    }
    
    Vec3f getColor() {
        return Vec3f(229, 229, 184) * (1.f/256);
    }
};

#endif /* rungekutta_integrator_hpp */
