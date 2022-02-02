#ifndef SPHERE_CLASS
#define SPHERE_CLASS

#include "object.hpp"
/**
 * @brief A sphere object.
 * 
 */
class Sphere : public Object {
public:
    Sphere(const std::string& name, double radius, Vector3d pos, Vector3d color, Vector3d emissivity, double reflectivity);
    /**
     * @return The radius of the sphere.
     */
    double GetRadius() const;
    Object_Type GetType() const { return SPHERE;}


private:
    double radius_;
};

#endif