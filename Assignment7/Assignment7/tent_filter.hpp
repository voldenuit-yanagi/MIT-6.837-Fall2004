#ifndef tent_filter_hpp
#define tent_filter_hpp

#include <stdio.h>

#include "filter.hpp"

class TentFilter: public virtual Filter {
private:
    float radius;
public:
    TentFilter(float r): radius(r) {}
    ~TentFilter() {}
    
    float getWeight(float x, float y) {
        float d = sqrt(x*x + y*y);
        if (d > radius) {
            return 0;
        }
        return 1 - d/radius;
    }
    int getSupportRadius() {
//        return floor(radius/sqrt(2) + 0.4999999);
        return floor(radius + 0.4999999);
    }
};

#endif /* tent_filter_hpp */
