#ifndef radial_force_field_hpp
#define radial_force_field_hpp

#include <stdio.h>

#include "forcefield.hpp"

class RadialForceField: public ForceField {
private:
    float magnitude;
public:
    RadialForceField(float _magnitude): magnitude(_magnitude) {}
    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const {
        return magnitude * (-1 * position) * (1.0f / mass);
    }
};

#endif /* radial_force_field_hpp */
