#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray &ray, const vec3 &intersection_point,
              const vec3 &normal, int recursion_depth) const {
    vec3 color;  // determine the color
    vec3 reflection_color = {0, 0, 0};

    if (recursion_depth < world.recursion_depth_limit) {// Calculate the reflection ray direction
        vec3 reflect_dir = ray.direction - 2 * dot(ray.direction, normal) * normal;
        reflect_dir = reflect_dir.normalized();
        // Check if the reflection ray intersects with any object in the scene
        Ray reflection_ray(intersection_point + reflect_dir * 0.01, reflect_dir);
        Hit reflection_hit = world.Closest_Intersection(reflection_ray);
        if (reflection_hit.dist < 10e5) {
            vec3 reflection_point = reflection_ray.Point(reflection_hit.dist);
            vec3 reflection_normal = reflection_hit.object->Normal(reflection_point, 0);// hit object normal
            reflection_color = reflection_hit.object->material_shader->Shade_Surface(reflection_ray, reflection_point,
                                                                                    reflection_normal,
                                                                                    recursion_depth + 1);
        }
    }
    // Call the shader's Shade_Surface function to get the initial color
    color = shader->Shade_Surface(ray, intersection_point, normal, recursion_depth);
    color = (1 - reflectivity) * color + reflectivity * reflection_color;
    return color;//if the reflection ray doesn't hit any object, return
}



