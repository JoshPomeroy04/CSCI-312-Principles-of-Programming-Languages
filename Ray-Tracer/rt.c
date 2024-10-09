/* Josh Pomeroy
 * 10/7/24
 * Ray Tracer Project
 * Creates a ray traced red sphere which is displayed in a ppm file
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rt.h"

// Main function which initializes structs and contains loop for each pixel
int main(void){
    // Initialize the sphere 
    SPHERE_T sphere;
    sphere.origin.x = sphere.origin.y = 0;
    sphere.origin.z = 10;
    sphere.radius = 2;
    
    // Initialize the sphere color
    COLOR_T sphere_color;
    sphere_color.R = 1;
    sphere_color.G = sphere_color.B = 0;

    // Initialize the ray
    RAY_T ray;
    ray.origin.x = ray.origin.y = ray.origin.z = 0;
    ray.dir.z = 1;

    // Initialize the light location
    VP_T light_loc;
    light_loc.x = 5;
    light_loc.y = 10;
    light_loc.z = 0;

    VP_T int_pt, normal;
    COLOR_T fill_color;
    double t;
    
    printf("P6\n1000\n1000\n255\n");
    for (double y = 0; y < 1000; y++){
        for (double x = 0; x < 1000; x++){
            ray.dir.x = -0.5 + x/1000;
            ray.dir.y = 0.5 - y/1000;
            normalize(&ray.dir);

            fill_color = trace(ray, sphere, &t, &int_pt, &normal, sphere_color, light_loc);
            printf("%c%c%c", (unsigned char) (fill_color.R * 255), (unsigned char) (fill_color.G * 255), (unsigned char) (fill_color.B * 255));
        }
    }

    return EXIT_SUCCESS;
}

/* Returns 1 if the provided ray intersects the given sphere, returns 0 if no intersection
 * If there is an intersection, calculates the normal and the intersection point and assigns
 * them to normal and int_pt
*/
int intersect_sphere(RAY_T ray, SPHERE_T sphere, double *t, VP_T *int_pt, VP_T *normal){
    double T1;
    double T2;
    int hit;

    double B = 2*(ray.dir.x * (ray.origin.x - sphere.origin.x) + 
                  ray.dir.y * (ray.origin.y - sphere.origin.y) +
                  ray.dir.z * (ray.origin.z - sphere.origin.z));

    double C = pow(ray.origin.x - sphere.origin.x, 2) + 
               pow(ray.origin.y - sphere.origin.y, 2) + 
               pow(ray.origin.z - sphere.origin.z, 2) -
               pow(sphere.radius, 2);
    double discriminant = pow(B, 2) - (4 * C);

    if (discriminant <= 0){
        hit = 0;
    } else {
        T1 = ((B * -1) + sqrt(discriminant)) / 2;
        T2 = ((B * -1) - sqrt(discriminant)) / 2;

        if (T1 > 0 && T1 < T2){                 // If T1 is smaller and greater than 0
            *t = T1;
            // Set intersection point
            int_pt->x = ray.origin.x + ray.dir.x * (*t);
            int_pt->y = ray.origin.y + ray.dir.y * (*t);
            int_pt->z = ray.origin.z + ray.dir.z * (*t);

            // Set normal
            normal->x = int_pt->x - sphere.origin.x;
            normal->y = int_pt->y - sphere.origin.y;
            normal->z = int_pt->z - sphere.origin.z;
            normalize(normal);

            hit = 1;
        } else if (T2 > 0 && T2 < T1){          // If T2 is smaller and greater than 0
            *t = T2;
            int_pt->x = ray.origin.x + ray.dir.x * (*t);
            int_pt->y = ray.origin.y + ray.dir.y * (*t);
            int_pt->z = ray.origin.z + ray.dir.z * (*t);

            normal->x = int_pt->x - sphere.origin.x;
            normal->y = int_pt->y - sphere.origin.y;
            normal->z = int_pt->z - sphere.origin.z;
            normalize(normal);

            hit = 1;
        } else {
            hit = 0;
        }
    }
    return hit;
}

// Returns a COLOR_T struct that contains the correct RGB values for the current pixel
COLOR_T illuminate(RAY_T ray, VP_T int_pt, COLOR_T obj_color, VP_T normal, VP_T light_loc){
    COLOR_T color;
    // Ambient
    color.R = 0.1 * obj_color.R;
    color.G = 0.1 * obj_color.G;
    color.B = 0.1 * obj_color.B;

    // Diffuse
    VP_T light, R;
    double dp, dp2;
    light.x = light_loc.x - int_pt.x;
    light.y = light_loc.y - int_pt.y;
    light.z = light_loc.z - int_pt.z;
    normalize(&light);
    dp = dot(light, normal);

    if (dp > 0){
        color.R += dp * obj_color.R;
        color.G += dp * obj_color.G;
        color.B += dp * obj_color.B;

        // Specular
        R.x = light.x - normal.x * 2 * dp;
        R.y = light.y - normal.y * 2 * dp;
        R.z = light.z - normal.z * 2 * dp;
        normalize(&R);
        dp2 = dot(R, ray.dir);
        if (dp2 > 0){
            color.R += pow(dp2, 200);
            color.G += pow(dp2, 200);
            color.B += pow(dp2, 200);
        }
    }

    // Caps each color to 1
    if (color.R > 1){
        color.R = 1;
    }

    if (color.G > 1){
        color.G = 1;
    }

    if (color.B > 1){
        color.B = 1;
    }

    return color;
}

// Checks each ray for an intersection and returns the correct color for the pixel
COLOR_T trace(RAY_T ray, SPHERE_T sphere, double *t, VP_T *int_pt, VP_T *normal, COLOR_T sphere_color, VP_T light_loc){
    COLOR_T color;

    if(intersect_sphere(ray, sphere, t, int_pt, normal)){
        color = illuminate(ray, *int_pt, sphere_color, *normal, light_loc);
    } else{
        color.R = 0;
        color.G = 0;
        color.B = 0;
    }

    return color;
}