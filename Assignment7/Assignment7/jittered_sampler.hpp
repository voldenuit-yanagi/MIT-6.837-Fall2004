#ifndef jittered_sampler_hpp
#define jittered_sampler_hpp

#include <stdio.h>

#include "sampler.hpp"

class JitteredSampler: public virtual Sampler {
private:
    int size;
    float d;
public:
    JitteredSampler(int _num_samples): Sampler(_num_samples) {
        size = sqrt(num_samples);
        d = 0.9999f / size;
        srand((unsigned)time(NULL));
    }
    ~JitteredSampler() {}
    
    Vec2f getSamplePosition(int n) {
        Vec2f p = Vec2f((n % size + 0.5) * d, (n / size + 0.5) * d);
        float x = p.x() + ((float)rand() / RAND_MAX - 0.5) * d;
        float y = p.y() + ((float)rand() / RAND_MAX - 0.5) * d;
        x = x < 0 ? 0 : x;
        x = x > 1 ? 1 : x;
        y = y < 0 ? 0 : y;
        y = y > 1 ? 1 : y;
        return Vec2f(x, y);
    }
};

#endif /* jittered_sampler_hpp */
