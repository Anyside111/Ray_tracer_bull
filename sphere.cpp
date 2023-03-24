#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    vec3 oc = ray.endpoint - this->center;
    double A = dot(ray.direction,ray.direction);
    double B = 2.0 * dot(ray.direction,oc);
    double C = dot(oc,oc) - radius*radius;
    double equation = B*B -4.0*A*C;
    double dist;
    if (equation < 0) {
        dist = 10e5;
    } else {
        dist = (-B - sqrt(equation) ) / (2.0*A);
    }

    return {this,dist,0};
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    normal = point - center; // compute the normal direction
    normal = normal.normalized();
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    vec3 new_lo = this->center - vec3(this->radius, this->radius, this->radius);
    vec3 new_hi = this->center + vec3(this->radius, this->radius, this->radius); // calculate bounding box
    box.lo = new_lo;
    box.hi = new_hi;
    return box;
}

