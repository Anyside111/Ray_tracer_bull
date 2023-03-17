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
        vec3 lightDir = this->world.lights[i]->position - intersection_point;
        vec3 lightDir_n = lightDir.normalized();
        vec3 norm = normal.normalized();
        double ifShadow;
        ifShadow = Shadow_Calcu(intersection_point,lightDir);
        double light_attenuation =
                4 * M_PI * lightDir.magnitude_squared();
        c_diffuse += ifShadow * incident * fmax(0, dot(lightDir_n, norm)) * color_diffuse / light_attenuation;

        double p = this->specular_power;
        vec3 viewDir = (this->world.camera.position - intersection_point).normalized();
        vec3 halfwayDir = (lightDir_n + viewDir).normalized();
        //vec3 reflectDir = -lightDir - 2.0 * dot(-lightDir, norm) * norm;
        c_specular += ifShadow * incident * pow(fmax(dot(halfwayDir, norm), 0), p) * color_specular / light_attenuation;
    }
    //4*p fit to 20.txt
    color = c_ambient + c_diffuse + c_specular;
    return color;
}

double Phong_Shader::Shadow_Calcu(const vec3 &intersection_point,
                                  const vec3 lightDir) const {
    double Light_Obj = lightDir.magnitude();
    Ray ray = {lightDir+intersection_point,-lightDir};
    Hit hit = this->world.Closest_Intersection(ray);
    double depth = hit.dist;
    double bias = 0.05;
    if (depth + bias < Light_Obj){
        return 0.0;
    }else{
        return 1.0;
    }
}

