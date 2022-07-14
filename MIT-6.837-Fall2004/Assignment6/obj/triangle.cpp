#include "triangle.hpp"

#include <math.h>
#include "grid.hpp"
#include "raytracing_stats.hpp"

Triangle::Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) {
    A = a;
    B = b;
    C = c;
    p0 = a;
    e1 = b - a;
    e2 = c - a;
    material = m;
    Vec3f::Cross3(normal, e1, e2);
    normal.Normalize();
    
    Vec3f min_(min({A.x(), B.x(), C.x()}), min({A.y(), B.y(), C.y()}), min({A.z(), B.z(), C.z()}));
    Vec3f max_(max({A.x(), B.x(), C.x()}), max({A.y(), B.y(), C.y()}), max({A.z(), B.z(), C.z()}));
    box = new BoundingBox(min_, max_);
}

bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
    RayTracingStats::IncrementNumIntersections();
    
    Vec3f d = r.getDirection();
    Vec3f temp;
    Vec3f::Cross3(temp, d, normal);
    if (temp.Length() == 0) {
        return false;
    }
    Vec3f s = r.getOrigin() - p0;
    Vec3f s1, s2;
    Vec3f::Cross3(s1, d, e2);
    Vec3f::Cross3(s2, s, e1);
    float t = s2.Dot3(e2) / s1.Dot3(e1);
    float b1 = s1.Dot3(s) / s1.Dot3(e1);
    float b2 = s2.Dot3(d) / s1.Dot3(e1);
    float b0 = 1 - b1 - b2;
    if (t > tmin && t < h.getT() && b1 > 0 && b2 > 0 && b0 > 0) {
        h.set(t, getMeterial(), normal, r);
        return true;
    }
    else {
        return false;
    }
}

void Triangle::paint(void) {
    material->glSetMaterial();
    
    glBegin(GL_TRIANGLES);
    glNormal3f(normal.x(), normal.y(), normal.z());
    glVertex3f(A.x(), A.y(), A.z());
    glVertex3f(B.x(), B.y(), B.z());
    glVertex3f(C.x(), C.y(), C.z());
    glEnd();
}

void Triangle::insertIntoGrid(Grid *g, Matrix *m) {
    Vec3f vertices[3] = {A, B, C};
    BoundingBox *newBox;
    Object3D *obj = this;
    if (m) {
        obj = new Transform(*m, this);
        
        m->Transform(vertices[0]);
        m->Transform(vertices[1]);
        m->Transform(vertices[2]);
        newBox = new BoundingBox(vertices[0], vertices[0]);
        newBox->Extend(vertices[1]);
        newBox->Extend(vertices[2]);
    }
    else {
        newBox = box;
    }
    
    Vec3f gridMin = g->getBoundingBox()->getMin();
    Vec3f cell = g->getUnitCell();
    Vec3f triMin = newBox->getMin() - gridMin;
    Vec3f triMax = newBox->getMax() - gridMin;
    triMin.Divide(cell.x(), cell.y(), cell.z());
    triMax.Divide(cell.x(), cell.y(), cell.z());
    
    for (int i=max(0.f, floorf(floorf(triMin.x())==triMin.x() ? triMin.x()-1 : triMin.x())); i<min(1.f*g->get_nx(), ceilf(ceilf(triMax.x())==triMax.x() ? triMax.x()+1 : triMax.x())); i++) {
        for (int j=max(0.f, floorf(floorf(triMin.y())==triMin.y() ? triMin.y()-1 : triMin.y())); j<min(1.f*g->get_ny(), ceilf(ceilf(triMax.y())==triMax.y() ? triMax.y()+1 : triMax.y())); j++) {
            for (int k=max(0.f, floorf(floorf(triMin.z())==triMin.z() ? triMin.z()-1 : triMin.z())); k<min(1.f*g->get_nz(), ceilf(ceilf(triMax.z())==triMax.z() ? triMax.z()+1 : triMax.z())); k++) {
                g->addRecord(i, j, k, obj);
            }
        }
    }
}
