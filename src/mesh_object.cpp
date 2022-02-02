#include "mesh_object.hpp"

Mesh::Mesh(const std::string& name, Vector3d pos, Vector3d color, Vector3d emissivity, double reflectivity) : Object::Object(name, pos, color, emissivity, reflectivity) {
}

unsigned int Mesh::AddVertex(Vector3d relativeLocation){
    vertices_.push_back(relativeLocation);
    return vertices_.size();
}
unsigned int Mesh::AddFace(Face face){
    faces_.push_back(face);
    return faces_.size();
}

Vector3d Mesh::GetVertex(unsigned int index) const {
    return vertices_.at(index - 1);
}
const std::vector<Face>& Mesh::GetFaces(){
    return faces_;
}