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
        vec3 lightDir =  (this->world.lights[i]->position - intersection_point).normalized();
        double lightIntensity_factor = 4* M_PI *(this->world.lights[i]->position - intersection_point).magnitude_squared();
        vec3 norm = normal.normalized();
        c_diffuse += incident * fmax(0, dot(norm, lightDir)) *color_diffuse/lightIntensity_factor;

        double p = this->specular_power;
        vec3 viewDir = (this->world.camera.position - intersection_point).normalized();
        vec3 reflectDir = -lightDir - 2.0 * dot(-lightDir,norm)*norm;
        c_specular += incident * pow(fmax(dot(viewDir,reflectDir), 0) , p) *color_specular/lightIntensity_factor;
    }
    color =  c_ambient + c_diffuse + c_specular;
    return color;
}
