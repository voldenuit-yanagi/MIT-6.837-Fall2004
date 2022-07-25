#ifndef gaussian_filter_hpp
#define gaussian_filter_hpp

#include <stdio.h>

#include "filter.hpp"

class GaussianFilter: public virtual Filter {
private:
    float sigma;
public:
    GaussianFilter(float s): sigma(s) {}
    ~GaussianFilter() {}
    
    float getWeight(float x, float y) {
        float d2 = x*x + y*y;
//        if (d2 > 4*sigma*sigma) {
//            return 0;
//        }
        return exp(-d2/(2*sigma*sigma));
    }
    int getSupportRadius() {
//        return ceil(2*sigma);
        return floor(2*sigma);
    }
};

#endif /* gaussian_filter_hpp */
