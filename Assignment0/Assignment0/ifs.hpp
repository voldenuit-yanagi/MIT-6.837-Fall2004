#ifndef IFS_HPP
#define IFS_HPP

#include <stdio.h>

#include "matrix.hpp"
#include "image.hpp"

class IFS {
    private:
        int n; // number of transformations
        Matrix *transformations;
        float *probabilities;
        float *accumulation;
    public:
        IFS(int num_transforms) {
            n = num_transforms;
            transformations = new Matrix[n];
            probabilities = new float[n];
            accumulation = new float[n];
        }
        void readDescription(int i, Matrix t, float p);
        void render(Image& image, int pointNumber, int iterationNumber);
        ~IFS() {
            delete [] transformations;
            delete [] probabilities;
        }
};

#endif
