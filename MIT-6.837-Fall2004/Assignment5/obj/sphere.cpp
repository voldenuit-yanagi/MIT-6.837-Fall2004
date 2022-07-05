#include "sphere.hpp"
#include "extern.hpp"
#include <math.h>
#include "grid.hpp"

Sphere::Sphere(Vec3f &c, float r, Material* m): Object3D() {
    center = c;
    radius = r;
    material = m;
    
    Vec3f vecR(radius, radius, radius);
    box = new BoundingBox(center-vecR, center+vecR);
}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f r0 = center - r.getOrigin();
    float length = r.getDirection().Length();
    float tp = r0.Dot3(r.getDirection())/length;
    float d2 = r0.Dot3(r0) - tp*tp;
    if (d2 > (radius*radius)) {
        return false;
    }
    float tpp = sqrt(radius*radius - d2);
    float t1 = (tp - tpp)/length;
    float t2 = (tp + tpp)/length;
    if (t1 <= tmin) {
        if (t2 > tmin && t2 < h.getT()) {
            Vec3f normal = t2 * r.getDirection() - r0;
            normal.Normalize();
            h.set(t2, getMeterial(), normal, r);
            return true;
        }
        else {
            return false;
        }
    }
    else if (t1 < h.getT()) {
        Vec3f normal = t1 * r.getDirection() - r0;
        normal.Normalize();
        h.set(t1, getMeterial(), normal, r);
        return true;
    }
    else {
        return false;
    }
}

void Sphere::paint(void) {
    material->glSetMaterial();
    
    float phi_step = M_PI / phi_steps;
    float theta_step = 2 * M_PI / theta_steps;
    
    Vec3f top(0, radius, 0);
    Vec3f bottom(0, -radius, 0);
    Vec3f **points = new Vec3f* [phi_steps+1];
    
    for (int iPhi=0; iPhi<phi_steps+1; iPhi++) {
        points[iPhi] = new Vec3f [theta_steps];
        for (int iTheta=0; iTheta<theta_steps; iTheta++) {
            if (iPhi == 0) {
                points[iPhi][iTheta] = center + top;
            }
            else if (iPhi == phi_steps) {
                points[iPhi][iTheta] = center + bottom;
            }
            points[iPhi][iTheta] = center + Vec3f(radius*cos(-M_PI/2+phi_step*iPhi)*cos(0+theta_step*iTheta), radius*sin(-M_PI/2+phi_step*iPhi), radius*cos(-M_PI/2+phi_step*iPhi)*sin(0+theta_step*iTheta));
        }
    }
    
    glBegin(GL_QUADS);
    for (int iPhi=0; iPhi<phi_steps; iPhi++) {
        for (int iTheta=0; iTheta<theta_steps; iTheta++) {
            // compute appropriate coordinates & normals
            Vec3f p1 = points[iPhi][iTheta];
            Vec3f p2 = points[iPhi+1][iTheta];
            Vec3f p3 = points[iPhi+1][(iTheta+1)%theta_steps];
            Vec3f p4 = points[iPhi][(iTheta+1)%theta_steps];
            Vec3f ps[4] = {p1, p2, p3, p4};
            
            Vec3f normal;
            if (!gouraud) {
                Vec3f barycenter = (p1 + p2 + p3 + p4) * 0.25;
                normal = barycenter - center;
                normal.Normalize();
                glNormal3f(normal.x(), normal.y(), normal.z());
            }
            
            for (int i=0; i<4; i++) {
                if (gouraud) {
                    normal = ps[i] - center;
                    normal.Normalize();
                    glNormal3f(normal.x(), normal.y(), normal.z());
                }
                glVertex3f(ps[i].x(), ps[i].y(), ps[i].z());
            }
        }
    }
    glEnd();
}

void Sphere::insertIntoGrid(Grid *g, Matrix *m) {
    if (m) {
        m->Inverse(*m, 0.001);
    }
    float halfDiagonal = g->getVoxelHalfDiagonalLength();
    for (int i = 0; i < g->get_nx(); i++) {
        for (int j = 0; j < g->get_ny(); j++) {
            for (int k = 0; k < g->get_nz(); k++) {
                Vec3f voxelCenter = g->getVoxelCenter(i, j, k);
                if (m) {
                    m->Transform(voxelCenter);
                }
                if ((voxelCenter-center).Length() - halfDiagonal < radius) {
                    g->addRecord(i, j, k, this);
                }
            }
        }
    }
}
