#ifndef force_field_hpp
#define force_field_hpp

#include <stdio.h>

#include "vectors.hpp"

class ForceField {
private:
    //
public:
    ForceField() {}
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;
};

#endif /* force_field_hpp */
