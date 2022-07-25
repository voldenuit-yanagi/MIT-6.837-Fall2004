#ifndef wind_force_field_hpp
#define wind_force_field_hpp

#include <stdio.h>

#include "forcefield.hpp"

class WindForceField: public ForceField {
private:
    float magnitude;
public:
    WindForceField(float _magnitude): magnitude(_magnitude) {}
    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const {
        float d = sqrt(position.x()*position.x() + position.z()*position.z());
        return magnitude * Vec3f(sin(d), 0, cos(d)) * (1.0f / mass);
    }
};

#endif /* wind_force_field_hpp */
