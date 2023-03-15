#include "light.h"
#include "object.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Phong_Shader::
Shade_Surface(const Ray &ray, const vec3 &intersection_point,
              const vec3 &normal, int recursion_depth) const {
    vec3 color;
    vec3 c_ambient = this->world.ambient_color * this->world.ambient_intensity * color_ambient;
    vec3 c_diffuse;
    vec3 c_specular;
    for (int i = 0; i < this->world.lights.size(); i++) {
        auto incident = this->world.lights[i]->brightness * this->world.lights[i]->color;
        vec3 L =  (ray.endpoint - intersection_point).normalized();
        c_diffuse += incident * fmax(0, dot(normal, L)) * color_diffuse/ (ray.endpoint - intersection_point).magnitude_squared();

        double p = this->specular_power;
        vec3 V = (intersection_point - ray.endpoint).normalized();
        vec3 R = 2.0 * dot(V, normal) * normal - V;
        c_specular += incident * fmax(0, pow(dot(R, V), p));

    }


    //color = c_ambient + c_diffuse + c_specular;
    color =  c_diffuse;
    return color;
}
