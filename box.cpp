#include <limits>
#include "box.h"

// Return whether the ray intersects this box.
bool Box::Intersection(const Ray &ray) const {
    //the slab intersection algorithm
    //computing the distance at which the ray intersects the box in the x, y, and z directions.
    // If the maximum of the minimum distances is less than the minimum of the maximum distances,
    // then the ray intersects the box.
    double tmin, tmax;

    // Compute distances to the x-axis slabs
    double tx1 = (lo[0] - ray.endpoint[0]) / ray.direction[0];
    double tx2 = (hi[0] - ray.endpoint[0]) / ray.direction[0];

    // Swap tx1 and tx2 if necessary to make tx1 the smaller distance
    if (tx1 > tx2) {
        std::swap(tx1, tx2);
    }

    // Compute distances to the y-axis slabs
    double ty1 = (lo[1] - ray.endpoint[1]) / ray.direction[1];
    double ty2 = (hi[1] - ray.endpoint[1]) / ray.direction[1];

    // Swap ty1 and ty2 if necessary to make ty1 the smaller distance
    if (ty1 > ty2) {
        std::swap(ty1, ty2);
    }

    // Compute distances to the z-axis slabs
    double tz1 = (lo[2] - ray.endpoint[2]) / ray.direction[2];
    double tz2 = (hi[2] - ray.endpoint[2]) / ray.direction[2];

    // Swap tz1 and tz2 if necessary to make tz1 the smaller distance
    if (tz1 > tz2) {
        std::swap(tz1, tz2);
    }

    // Compute the minimum and maximum distances
    tmin = std::max(std::max(tx1, ty1), tz1);
    tmax = std::min(std::min(tx2, ty2), tz2);

    // Check if the ray intersects the box
    return tmax >= tmin && tmax >= 0;
}

// Compute the smallest box that contains both *this and bb.
Box Box::Union(const Box &bb) const {
    Box box;
    vec3 new_lo(std::min(lo[0], bb.lo[0]), std::min(lo[1], bb.lo[1]), std::min(lo[2], bb.lo[2]));
    vec3 new_hi(std::max(hi[0], bb.hi[0]), std::max(hi[1], bb.hi[1]), std::max(hi[2], bb.hi[2]));
    box.lo = new_lo;
    box.hi = new_hi;
    return box;
}

// Enlarge this box (if necessary) so that pt also lies inside it.
void Box::Include_Point(const vec3 &pt) {
    lo[0] = std::min(lo[0], pt[0]);
    lo[1] = std::min(lo[1], pt[1]);
    lo[2] = std::min(lo[2], pt[2]);
    hi[0] = std::max(hi[0], pt[0]);
    hi[1] = std::max(hi[1], pt[1]);
    hi[2] = std::max(hi[2], pt[2]);
}

// Create a box to which points can be correctly added using Include_Point.
void Box::Make_Empty() {
    lo.fill(std::numeric_limits<double>::infinity());
    hi = -lo;
}

// Create a box that contains everything.
void Box::Make_Full() {
    hi.fill(std::numeric_limits<double>::infinity());
    lo = -hi;
}
