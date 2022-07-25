#ifndef checker_board_hpp
#define checker_board_hpp

#include <stdio.h>

#include "material.hpp"
#include "matrix.hpp"

class CheckerBoard: public virtual Material {
private:
    Matrix *mappingMatrix;
    Material *material1;
    Material *material2;
public:
    CheckerBoard(Matrix *m, Material *mat1, Material *mat2): Material(Vec3f(1, 1, 1)), mappingMatrix(m), material1(mat1), material2(mat2) {}
    ~CheckerBoard() { delete mappingMatrix; delete material1; delete material2; }
    
    Vec3f getDiffuseColor(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        bool which = int(floor(point[0]) + floor(point[1]) + floor(point[2])) & 1;
        if (!which) {
            return material1->getDiffuseColor();
        }
        else {
            return material2->getDiffuseColor();
        }
    }
    Vec3f getSpecularColor(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        bool which = int(floor(point[0]) + floor(point[1]) + floor(point[2])) & 1;
        if (!which) {
            return material1->getSpecularColor();
        }
        else {
            return material2->getSpecularColor();
        }
    }
    Vec3f getReflectiveColor(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        bool which = int(floor(point[0]) + floor(point[1]) + floor(point[2])) & 1;
        if (!which) {
            return material1->getReflectiveColor();
        }
        else {
            return material2->getReflectiveColor();
        }
    }
    Vec3f getTransparentColor(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        bool which = int(floor(point[0]) + floor(point[1]) + floor(point[2])) & 1;
        if (!which) {
            return material1->getTransparentColor();
        }
        else {
            return material2->getTransparentColor();
        }
    }
    float getIndexOfRefraction(Vec3f point = Vec3f(0, 0, 0)) const {
        mappingMatrix->Transform(point);
        bool which = int(floor(point[0]) + floor(point[1]) + floor(point[2])) & 1;
        if (!which) {
            return material1->getIndexOfRefraction();
        }
        else {
            return material2->getIndexOfRefraction();
        }
    }
    
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
    void glSetMaterial(void) const;
};

#endif /* checker_board_hpp */
