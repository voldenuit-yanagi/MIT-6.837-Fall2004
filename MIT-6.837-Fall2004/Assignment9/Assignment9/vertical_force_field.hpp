#ifndef vertical_force_field_hpp
#define vertical_force_field_hpp

#include <stdio.h>

#include "forcefield.hpp"

class VerticalForceField: public ForceField {
private:
    float magnitude;
public:
    VerticalForceField(float _magnitude): magnitude(_magnitude) {}
    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const {
        return magnitude * (-1 * Vec3f(0, position.y(), 0)) * (1.0f / mass);
    }
};

#endif /* vertical_force_field_hpp */
