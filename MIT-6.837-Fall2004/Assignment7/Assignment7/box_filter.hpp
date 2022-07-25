#ifndef box_filter_hpp
#define box_filter_hpp

#include <stdio.h>

#include "filter.hpp"

class BoxFilter: public virtual Filter {
private:
    float radius;
public:
    BoxFilter(float r): radius(r) {}
    ~BoxFilter() {}
    
    float getWeight(float x, float y) {
        if (fabs(x) > radius || fabs(y) > radius) {
            return 0;
        }
        return 1;
    }
    int getSupportRadius() {
        return floor(radius+0.4999999);
    }
};

#endif /* box_filter_hpp */
