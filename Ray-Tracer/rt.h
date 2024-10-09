#ifndef RT_H
#define RT_H

#include "vp.h"

typedef struct{
    VP_T origin;
    VP_T dir;
} RAY_T;

typedef struct{
    double R;
    double G;
    double B;
} COLOR_T;

typedef struct{
    VP_T origin;
    double radius;
} SPHERE_T;

int intersect_sphere(RAY_T ray, SPHERE_T sphere, double *t, VP_T *int_pt, VP_T *normal);
COLOR_T illuminate(RAY_T ray, VP_T int_pt, COLOR_T obj_color, VP_T normal, VP_T light_loc);
COLOR_T trace(RAY_T ray, SPHERE_T sphere, double *t, VP_T *int_pt, VP_T *normal, COLOR_T sphere_color, VP_T light_loc);

#endif // RT_H