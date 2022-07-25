#ifndef curve_hpp
#define curve_hpp

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#include <stdio.h>
#include <vector>

#include "arg_parser.hpp"
#include "spline.hpp"

class Curve: public Spline {
protected:
    int num_vertices;
    vector<Vec3f> vertices;
public:
    Curve(int _num_vertices): num_vertices(_num_vertices), vertices(num_vertices) {}
    ~Curve() {}
    
    virtual void set(int i, Vec3f v) {
        vertices[i] = v;
    }
    
    virtual void drawCurve(int tessellation) = 0;
    
    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) {
        
        glColor3f(0.f, 0.f, 1.f);
        glLineWidth(3);
        glBegin(GL_LINE_STRIP);
        for (Vec3f &v: vertices) {
            glVertex3f(v.x(), v.y(), v.z());
        }
        glEnd();

        drawCurve(args->curve_tessellation);
        
        glColor3f(1.f, 0.f, 0.f);
        glPointSize(5);
        glBegin(GL_POINTS);
        for (Vec3f &v: vertices) {
            glVertex3f(v.x(), v.y(), v.z());
        }
        glEnd();
    }

    // FOR CONTROL POINT PICKING
    virtual int getNumVertices() {
        return num_vertices;
    }
    virtual Vec3f getVertex(int i) {
        return vertices[i];
    }

    // FOR EDITING OPERATIONS
    virtual void moveControlPoint(int selectedPoint, float x, float y) {
        vertices[selectedPoint].Set(x, y, 0);
    }
    virtual void addControlPoint(int selectedPoint, float x, float y) {
        int cnt = 4 - num_vertices % 4;
        vector<Vec3f>::iterator index = vertices.begin() + selectedPoint;
        for (int i=0; i<cnt; i++) {
            vertices.insert(index, Vec3f(x, y, 0));
        }
        num_vertices += cnt;
    }
    virtual void deleteControlPoint(int selectedPoint) {
        vector<Vec3f>::iterator index = vertices.begin() + selectedPoint;
        vertices.erase(index);
        num_vertices--;
        vertices.insert(index, vertices[selectedPoint]);
    }
    
    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file) = 0;
    virtual void OutputBSpline(FILE *file) = 0;

    // FOR GENERATING TRIANGLES
    virtual TriangleMesh* OutputTriangles(ArgParser *args) = 0;
};

#endif /* curve_hpp */
