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
    double dist=0;
    if (equation < 0) {
        dist = -1.0;
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
    TODO; // calculate bounding box
    return box;
}

