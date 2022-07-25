#ifndef random_sampler_hpp
#define random_sampler_hpp

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sampler.hpp"

class RandomSampler: public virtual Sampler {
private:
    //
public:
    RandomSampler(int _num_samples): Sampler(_num_samples) {
        srand((unsigned)time(NULL));
    }
    ~RandomSampler() {}
    
    Vec2f getSamplePosition(int n) {
        return Vec2f(rand()/float(RAND_MAX), rand()/float(RAND_MAX));
    }
};

#endif /* random_sampler_hpp */
