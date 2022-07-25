#ifndef constant_force_field_hpp
#define constant_force_field_hpp

#include <stdio.h>

#include "forcefield.hpp"

class ConstantForceField: public ForceField {
private:
    Vec3f force;
public:
    ConstantForceField(Vec3f _force): force(_force) {}
    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const {
        return force * (1.0f / mass);
    }
};

#endif /* constant_force_field_hpp */
