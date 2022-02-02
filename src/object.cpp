#include "object.hpp"

Object::Object(const std::string& name, Vector3d pos, Vector3d color, Vector3d emissivity, double reflectivity) { 
    name_ = name;
    pos_ = pos;
    color_ = color;
    emissivity_ = emissivity;
    reflectivity_ = reflectivity;
}

const std::string& Object::GetName() const {
    return name_;
}

Vector3d Object::GetPosition() const {
    return pos_;
}

Vector3d Object::GetColor() const {
    return color_;
}

Vector3d Object::GetEmissivityColor() const {
    return emissivity_;
}

double Object::GetReflectivity() const {
    return reflectivity_;
}