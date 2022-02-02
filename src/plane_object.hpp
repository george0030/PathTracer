#ifndef PLANE_CLASS
#define PLANE_CLASS

#include "object.hpp"

class Plane : public Object {
public:
    Plane(const std::string& name, Vector3d pos, Vector3d normal, Vector3d color, Vector3d emissivity, double reflectivity);
    /**
    * Return the normal vector of this plane.
    * @return Normal vector of this plane.
    */
    Vector3d GetNormal() const;
    Object_Type GetType() const { return PLANE;}

private:
    Vector3d normal_;
    
};

#endif