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


    return {};
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
bool Mesh::Intersect_Triangle(const Ray &ray, int tri, double &dist) const {
//    if (dist>small_t && barycentric weights > -weight_tolerance){
//        dist=;
//        barycentric weights=;
//        return true;

    //return false;
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
