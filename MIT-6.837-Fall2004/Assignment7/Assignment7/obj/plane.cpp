#include "plane.hpp"
#include "raytracing_stats.hpp"
#include "grid.hpp"

bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
    RayTracingStats::IncrementNumIntersections();
    
    Vec3f rd = r.getDirection();
    Vec3f temp;
    Vec3f::Cross3(temp, rd, normal);
    if (temp.Length() == 0) {
        return false;
    }
    else {
        Vec3f r0 = r.getOrigin();
        float t = -(D + r0.Dot3(normal))/rd.Dot3(normal);
        if (t > tmin && t < h.getT()) {
            h.set(t, getMeterial(), normal, r);
            return true;
        }
        else {
            return false;
        }
    }
}

void Plane::paint(void) {
    material->glSetMaterial();
    
    Vec3f v = (normal == Vec3f(1, 0, 0)) ? Vec3f(0, 1, 0) : Vec3f(1, 0, 0);
    Vec3f b1, b2;
    Vec3f::Cross3(b1, v, normal);
    Vec3f::Cross3(b2, normal, b1);
    
    Vec3f projected_origin = -D / pow(normal.Length(), 2) * normal;
    Vec3f vertices[4] = { projected_origin + b1 * 59999, projected_origin + b2*59999, projected_origin - b1*59999, projected_origin - b2*59999 };
    
    glBegin(GL_QUADS);
    glNormal3f(normal.x(), normal.y(), normal.z());
    for (int i = 0; i < 4; i++) {
        glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
    }
    glEnd();
}

void Plane::insertIntoGrid(Grid *g, Matrix *m) {
    if (m) {
        g->addPlaneRecord(new Transform(*m, this));
    }
    else {
        g->addPlaneRecord(this);
    }
}
