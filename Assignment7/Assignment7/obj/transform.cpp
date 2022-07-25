#include "transform.hpp"

Transform::Transform(Matrix &m, Object3D *o) {
    mat = m;
    obj = o;
    
    BoundingBox *b = o->getBoundingBox();
    if (b) {
        Vec3f vec[2];
        vec[0] = b->getMin();
        vec[1] = b->getMax();
        for (int i = 1; i <= 8; i++)
        {
            Vec3f v = Vec3f(vec[i & 1].x(), vec[(i >> 1) & 1].y(), vec[(i >> 2) & 1].z());
            m.Transform(v);
            if (box)
                box->Extend(v);
            else
                box = new BoundingBox(v, v);
        }
    }
}

bool Transform::intersect(const Ray &r, Hit &h, float tmin) {
    Vec4f ori(r.getOrigin(), 1.0);
    Vec4f dir(r.getDirection(), 0.0);
    
    Matrix temp(mat);
    temp.Inverse(temp, 0.001);
    temp.Transform(ori);
    temp.Transform(dir);
    
    Vec3f ori3(ori.x(), ori.y(), ori.z());
    Vec3f dir3(dir.x(), dir.y(), dir.z());
    
    if (obj->intersect(Ray(ori3, dir3), h, tmin)) {
        Vec4f normal(h.getNormal(), 0.0);
        temp.Transpose();
        temp.Transform(normal);
        Vec3f normal3(normal.x(), normal.y(), normal.z());
        normal3.Normalize();
        h.set(h.getT(), h.getMaterial(), normal3, r);
        return true;
    }
    return false;
}

void Transform::paint(void) {
    glPushMatrix();
    GLfloat *glMatrix = mat.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;

    obj->paint();
    glPopMatrix();
}

void Transform::insertIntoGrid(Grid *g, Matrix *m) {
    if (m) {
        Matrix newMat = (*m) * mat;
        obj->insertIntoGrid(g, &newMat);
    }
    else {
        obj->insertIntoGrid(g, &mat);
    }
}
