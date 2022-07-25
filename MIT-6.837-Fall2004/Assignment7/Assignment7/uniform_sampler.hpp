#ifndef uniform_sampler_hpp
#define uniform_sampler_hpp

#include <stdio.h>

#include "sampler.hpp"

class UniformSampler: public virtual Sampler {
private:
    int size;
    float d;
public:
    UniformSampler(int _num_samples): Sampler(_num_samples) {
        size = sqrt(num_samples);
        d = 0.9999f / size;
    }
    ~UniformSampler() {}
    
    Vec2f getSamplePosition(int n) {
        if (num_samples == 1) {
            return Vec2f(0.5, 0.5);
        }
        else {
            return Vec2f((n % size + 0.5) * d, (n / size + 0.5) * d);
        }
    }
};

#endif /* uniform_sampler_hpp */
