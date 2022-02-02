#include "plane_object.hpp"

Plane::Plane(const std::string& name, Vector3d pos, Vector3d normal, Vector3d color, Vector3d emissivity, double reflectivity) : normal_(normal), Object::Object(name, pos, color, emissivity, reflectivity) {}


Vector3d Plane::GetNormal() const {
    return normal_;
}