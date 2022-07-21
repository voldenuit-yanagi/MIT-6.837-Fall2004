#ifndef gravity_force_field_hpp
#define gravity_force_field_hpp

#include <stdio.h>

#include "forcefield.hpp"

class GravityForceField: public ForceField {
private:
    Vec3f gravity;
public:
    GravityForceField(Vec3f _gravity): gravity(_gravity) {}
    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const {
        return gravity;
    }
};

#endif /* gravity_force_field_hpp */
