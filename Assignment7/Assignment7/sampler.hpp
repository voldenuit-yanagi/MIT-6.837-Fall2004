#ifndef sampler_hpp
#define sampler_hpp

#include <stdio.h>

#include "vectors.hpp"

class Sampler {
protected:
    int num_samples;
public:
    Sampler(int _num_samples): num_samples(_num_samples) {}
    ~Sampler() {}
    virtual Vec2f getSamplePosition(int n) = 0;
};

#endif /* sampler_hpp */
