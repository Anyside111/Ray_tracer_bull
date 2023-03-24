#include "mesh.h"
#include <fstream>
#include <string>
#include <limits>


// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char *file) {
    std::ifstream fin(file);
    if (!fin) {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while (fin) {
        getline(fin, line);

        if (sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3) {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if (sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3) {
            for (int i = 0; i < 3; i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts = triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
// For meshes, the part structure attribute should be set to the
// index of the triangle that was intersected.
//iterate over every boxes?
Hit Mesh::Intersection(const Ray &ray, int part) const {
    assert(part >= 0 && part < triangles.size());// assert(part<0), intersect against all parts.
    double dist;
    Hit hit;
    if (Intersect_Triangle(ray, part, dist)) {
        // intersection point inside :Compute the distance from the origin of the ray to the point of intersection
        vec3 inter_point = ray.Point(dist);//use this dist to compute the intersection point
        double hit_dist = (inter_point - ray.endpoint).magnitude();
        hit = {this, hit_dist, part};
        return hit;
    }
    return {nullptr, 0, 0};
}

vec3 Mesh::Barycentric(const vec3 &p, const vec3 &a, const vec3 &b, const vec3 &c) const {
    const auto v0 = b - a;
    const auto v1 = c - a;
    const auto v2 = p - a;
    const auto d00 = dot(v0, v0);
    const auto d01 = dot(v0, v1);
    const auto d11 = dot(v1, v1);
    const auto d20 = dot(v2, v0);
    const auto d21 = dot(v2, v1);
    const auto dom = d00 * d11 - d01 * d01;
    const auto v = (d11 * d20 - d01 * d21) / dom;
    const auto w = (d00 * d21 - d01 * d20) / dom;
    const auto u = 1.0f - v - w;
    return {u, v, w};
}


// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3 &point, int part) const {
    //assert(part>=0);
    assert(part >= 0 && part < triangles.size());
    const auto &tri = this->triangles[part];
    vec3 v1 = this->vertices[tri[1]] - this->vertices[tri[0]];//double ,3
    vec3 v2 = this->vertices[tri[2]] - this->vertices[tri[0]];
    vec3 normal = cross(v1, v2);
    return vec3(normal.normalized());
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
//an integer tri which represents the index of the triangle in the Mesh object's triangles vector
bool Mesh::Intersect_Triangle(const Ray &ray, int tri, double &dist) const {
    const auto &vertices = this->vertices;// all vertices read from file
    const auto &indices = this->triangles[tri]; // indices of the vertices of the specified triangle[tri]

    // Compute the normal of the triangle
    vec3 v0 = vertices[indices[0]];
    vec3 v1 = vertices[indices[1]];
    vec3 v2 = vertices[indices[2]];
    vec3 normal = cross(v1 - v0, v2 - v0).normalized();

    // Test for intersection with the plane of the triangle,refer to the plane.cpp
    double dom = dot(ray.direction, normal);
    if (std::abs(dom) < 10e-8) {
        // Ray is parallel to the plane
        return false;
    }
    double tmp_dist = dot(v0 - ray.endpoint, normal) / dom;

    if (tmp_dist <= small_t) {
        // Intersection point is behind the ray origin
        return false;
    }

    // Compute the intersection point
    vec3 inter_point = ray.endpoint + tmp_dist * ray.direction;

    // Compute the barycentric coordinates of the intersection point
    vec3 w = Barycentric(inter_point, v0, v1, v2);

    // Check if the intersection point is inside the triangle
    //constexpr double weight_tolerance = -1e-8;
    if (w[0] >= -weight_tolerance && w[1] >= -weight_tolerance && w[2] >= -weight_tolerance) {
        dist = tmp_dist;
        return true;
    }
    return false;
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const {
    assert(part >= 0 && part < triangles.size());// assert(part<0) return the bounding box for the whole object.
    const auto &tri = triangles[part];
    vec3 new_lo(
            std::min(vertices[tri[0]][0], std::min(vertices[tri[1]][0], vertices[tri[2]][0])),
            std::min(vertices[tri[0]][1], std::min(vertices[tri[1]][1], vertices[tri[2]][1])),
            std::min(vertices[tri[0]][2], std::min(vertices[tri[1]][2], vertices[tri[2]][2]))
    );
    vec3 new_hi(
            std::max(vertices[tri[0]][0], std::max(vertices[tri[1]][0], vertices[tri[2]][0])),
            std::max(vertices[tri[0]][1], std::max(vertices[tri[1]][1], vertices[tri[2]][1])),
            std::max(vertices[tri[0]][2], std::max(vertices[tri[1]][2], vertices[tri[2]][2]))
    );

    Box b;
    b.hi = new_hi;
    b.lo = new_lo;
    return b;
}
