#ifndef filter_hpp
#define filter_hpp

#include <stdio.h>

class Filter {
private:
    //
public:
    Filter() {}
    ~Filter() {}
    
    Vec3f getColor(int i, int j, Film *film) {
        Vec3f color(0, 0, 0);
        int supportRadius = getSupportRadius();
        int width = film->getWidth();
        int height = film->getHeight();
        float total_weight = 0;
        for (int ii=i-supportRadius; ii<=i+supportRadius; ii++) {
            for (int jj=j-supportRadius; jj<=j+supportRadius; jj++) {
                if (ii < 0 || ii >= width || jj < 0 || jj >= height) continue;
                for (int n=0; n<film->getNumSamples(); n++) {
                    Sample s = film->getSample(ii, jj, n);
                    float x = ii + s.getPosition().x() - (i + 0.5);
                    float y = jj + s.getPosition().y() - (j + 0.5);
                    float w = getWeight(x, y);
                    color += w * s.getColor();
                    total_weight += w;
                }
            }
        }
        return color * (1.f/total_weight);
    }
    virtual float getWeight(float x, float y) = 0;
    virtual int getSupportRadius() = 0;
};

#endif /* filter_hpp */
