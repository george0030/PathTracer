#include "sphere_object.hpp"

Sphere::Sphere(const std::string& name, double radius, Vector3d pos, Vector3d color, Vector3d emissivity, double reflectivity) : Object::Object(name, pos, color, emissivity, reflectivity) {
    radius_ = radius;
}

double Sphere::GetRadius() const {
    return radius_;
}