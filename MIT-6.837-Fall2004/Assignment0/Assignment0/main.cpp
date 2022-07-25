#include <iostream>
#include <stdio.h>
using namespace std;

#include "image.hpp"
#include "ifs.hpp"

int main(int argc, char * argv[]) {
    
    char *input_file = NULL;
    int num_points = 10000;
    int num_iters = 10;
    int size = 100;
    char *output_file = NULL;

    // sample command line:
    // ifs -input fern.txt -points 10000 -iters 10 -size 100 -output fern.tga
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc);
            input_file = argv[i];
        } else if (!strcmp(argv[i],"-points")) {
            i++; assert (i < argc);
            num_points = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-iters")) {
            i++; assert (i < argc);
            num_iters = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-size")) {
            i++; assert (i < argc);
            size = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-output")) {
            i++; assert (i < argc);
            output_file = argv[i];
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }

    Image image(size, size);
    Vec3f backgroundColor(1, 1, 1);
    image.SetAllPixels(backgroundColor);

    // open the file
    FILE *input = fopen(input_file, "r");
    assert(input != NULL);

    // read the number of transforms
    int num_transforms;
    fscanf(input, "%d", &num_transforms);

    IFS ifs(num_transforms);

    // read in the transforms
    for (int i = 0; i < num_transforms; i++) {
        float probability;
        fscanf (input, "%f", &probability);
        Matrix m;
        m.Read3x3(input);
        ifs.readDescription(i, m, probability);
    }

    // close the file
    fclose(input);

    ifs.render(image, num_points, num_iters);

    image.SaveTGA(output_file);

    return 0;
}
