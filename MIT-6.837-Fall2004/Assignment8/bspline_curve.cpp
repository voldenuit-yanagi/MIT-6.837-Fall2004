#include "bspline_curve.hpp"

void BSplineCurve::drawCurve(int tessellation) {
    glColor3f(0.f, 1.f, 0.f);
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    float delta_t = 1. / tessellation;
    for (int i=0; i<num_vertices-3; i++) {
        Matrix G_bspline;
        for (int j=0; j<4; j++) {
            G_bspline.Set(j, 0, vertices[i+j].x());
            G_bspline.Set(j, 1, vertices[i+j].y());
            G_bspline.Set(j, 2, vertices[i+j].z());
        }
        for (int k=0; k<=tessellation; k++) {
            float t = k*delta_t;
            Vec4f S(t*t*t, t*t, t, 1);
            BSpline_Matrix.Transform(S);
            G_bspline.Transform(S);
            glVertex3f(S.x(), S.y(), S.z());
        }
    }
    glEnd();
}

void BSplineCurve::OutputBezier(FILE *file) {
    fprintf(file, "\n");
    fprintf(file, "bezier\nnum_vertices %d\n", num_vertices);
    Matrix G_bspline;
    for (int i=0; i<num_vertices; i++) {
        G_bspline.Set(i, 0, vertices[i].x());
        G_bspline.Set(i, 1, vertices[i].y());
        G_bspline.Set(i, 2, vertices[i].z());
    }
    Matrix inv_Bezier_Matrix = Bezier_Matrix;
    inv_Bezier_Matrix.Inverse();
    Matrix G_bezier = G_bspline * BSpline_Matrix * inv_Bezier_Matrix;
    for (int i=0; i<num_vertices; i++) {
        fprintf(file, "%f %f %f\n", G_bezier.Get(i, 0), G_bezier.Get(i, 1), G_bezier.Get(i, 2));
    }
}

void BSplineCurve::OutputBSpline(FILE *file) {
    fprintf(file, "\n");
    fprintf(file, "bspline\nnum_vertices %d\n", num_vertices);
    for (Vec3f &v: vertices) {
        fprintf(file, "%f %f %f\n", v.x(), v.y(), v.z());
    }
}

TriangleMesh* BSplineCurve::OutputTriangles(ArgParser *args) {
    int curve_tessellation = args->curve_tessellation;
    int revolution_tessellation = args->revolution_tessellation;
    float curve_d = 1.0f / curve_tessellation;
    float revolution_d = 2.0 * M_PI / revolution_tessellation;
    TriangleNet *triangles = new TriangleNet(revolution_tessellation, (num_vertices-3)*curve_tessellation);
    
    for (int i=0; i<=revolution_tessellation; i++) {
        float theta = i * revolution_d;
        for (int j=0; j<num_vertices-3; j++) {
            Matrix G_bspline;
            for (int k=0; k<4; k++) {
                G_bspline.Set(k, 0, vertices[j+k].x());
                G_bspline.Set(k, 1, vertices[j+k].y());
                G_bspline.Set(k, 2, vertices[j+k].z());
            }
            for (int k=0; k<=curve_tessellation; k++) {
                float t = k * curve_d;
                Vec4f S(t*t*t, t*t, t, 1);
                BSpline_Matrix.Transform(S);
                G_bspline.Transform(S);
                
                float x = cos(theta) * S.x() + sin(theta) * S.z();
                float y = S.y();
                float z = -sin(theta) * S.x() + cos(theta) * S.z();
                triangles->SetVertex(i, (j / 3 * curve_tessellation) + k, Vec3f(x, y, z));
            }
        }
    }
    return triangles;
}
