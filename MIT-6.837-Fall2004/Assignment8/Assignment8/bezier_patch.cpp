#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#include "bezier_patch.hpp"

void BezierPatch::Paint(ArgParser *args) {
    glColor3f(1.f, 0.f, 0.f);
    glPointSize(5);
    glBegin(GL_POINTS);
    for (Vec3f &v: vertices) {
        glVertex3f(v.x(), v.y(), v.z());
    }
    glEnd();
    
    glColor3f(0.f, 0.f, 1.f);
    glLineWidth(3);
    for (int i=0; i<sqrt(num_vertices); i++) {
        glBegin(GL_LINE_STRIP);
        for (int j=0; j<sqrt(num_vertices); j++) {
            Vec3f v = vertices[i*sqrt(num_vertices)+j];
            glVertex3f(v.x(), v.y(), v.z());
        }
        glEnd();
    }
    for (int i=0; i<sqrt(num_vertices); i++) {
        for (int j=0; j<sqrt(num_vertices); j++) {
            Vec3f v = vertices[j*sqrt(num_vertices)+i];
            glVertex3f(v.x(), v.y(), v.z());
        }
        glEnd();
    }
    
    glColor3f(0.f, 1.f, 0.f);
    glLineWidth(3);
    float patch_tessellation = args->patch_tessellation;
    float delta_t = 1. / patch_tessellation;
    
    Matrix G[4];
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            G[i].Set(j, 0, vertices[4*i+j].x());
            G[i].Set(j, 1, vertices[4*i+j].y());
            G[i].Set(j, 2, vertices[4*i+j].z());
        }
    }
    
    for (int i=0; i<=patch_tessellation; i++) {
        float t = i*delta_t;
        Matrix G_bezier;
        for (int j=0; j<4; j++) {
            Vec4f T(t*t*t, t*t, t, 1);
            Bezier_Matrix.Transform(T);
            G[j].Transform(T);
            
            G_bezier.Set(j, 0, T.x());
            G_bezier.Set(j, 1, T.y());
            G_bezier.Set(j, 2, T.z());
        }
        glBegin(GL_LINE_STRIP);
        for (int k=0; k<=patch_tessellation; k++) {
            float s = k*delta_t;
            Vec4f S(s*s*s, s*s, s, 1);
            Bezier_Matrix.Transform(S);
            G_bezier.Transform(S);
            glVertex3f(S.x(), S.y(), S.z());
        }
        glEnd();
    }
}

TriangleMesh* BezierPatch::OutputTriangles(ArgParser *args) {
    float patch_tessellation = args->patch_tessellation;
    float delta_t = 1. / patch_tessellation;
    
    TriangleNet *triangles = new TriangleNet(patch_tessellation, patch_tessellation);
    
    Matrix G[4];
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            G[i].Set(j, 0, vertices[4*i+j].x());
            G[i].Set(j, 1, vertices[4*i+j].y());
            G[i].Set(j, 2, vertices[4*i+j].z());
        }
    }
    
    for (int i=0; i<=patch_tessellation; i++) {
        float t = i*delta_t;
        Matrix G_bezier;
        for (int j=0; j<4; j++) {
            Vec4f T(t*t*t, t*t, t, 1);
            Bezier_Matrix.Transform(T);
            G[j].Transform(T);
            
            G_bezier.Set(j, 0, T.x());
            G_bezier.Set(j, 1, T.y());
            G_bezier.Set(j, 2, T.z());
        }
        for (int k=0; k<=patch_tessellation; k++) {
            float s = k*delta_t;
            Vec4f S(s*s*s, s*s, s, 1);
            Bezier_Matrix.Transform(S);
            G_bezier.Transform(S);
            triangles->SetVertex(i, k, Vec3f(S.x(), S.y(), S.z()));
        }
    }
    return triangles;
}
