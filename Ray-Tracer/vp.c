/* Josh Pomeroy
 * 10/7/24
*/
#include <math.h>
#include "vp.h"

// Returns the length of a vector
double length(VP_T vector){
    return sqrt((vector.x * vector.x) + 
                (vector.y * vector.y) + 
                (vector.z * vector.z));
}

// Normalizes a vector
void normalize(VP_T *vector){
    double len = length(*vector);

    vector->x /= len;
    vector->y /= len;
    vector->z /= len;
}

// Returns the dot product of two vectors
double dot(VP_T v1, VP_T v2){
    return (v1.x * v2.x) +
           (v1.y * v2.y) +
           (v1.z * v2.z);
}