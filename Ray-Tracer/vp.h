#ifndef VP_H
#define VP_H

typedef struct{
    double x, y, z;
} VP_T;

void normalize(VP_T *vector);
double dot(VP_T v1, VP_T v2);

#endif // VP_H