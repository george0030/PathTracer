#ifndef OBJECT_CLASS
#define OBJECT_CLASS

#include <eigen-3.4.0/Eigen/Dense>

using namespace Eigen;
/**
 * @brief Represents object type. Used for type differentiation in Scene.Render().
 * @see Scene
 * 
 */
enum Object_Type {
    SPHERE, PLANE, MESH
};
/**
 * @brief Represents a renderable 3d object.
 * 
 */
class Object {
public:
/**
 * @brief Construct a new 3d object
 * 
 * @param name Object name.
 * @param pos Object origin location. For spheres, this is the center point.
 * @param color Color of the object, x=red, y=green, z=blue.
 * @param emissivity The color of the light that this object emits. 0,0,0 = black
 * @param reflectivity Amount of light this object reflects. 0.0 = none, 1.0 = mirror
 */
    Object(const std::string& name, Vector3d pos, Vector3d color, Vector3d emissivity, double reflectivity);

    const std::string& GetName() const;
    /**
     * @return Object origin location
     */
    Vector3d GetPosition() const;
    /**
     * @return Object color
     */
    Vector3d GetColor() const;
    /**
     * @return The color of the light that this object emits. 0,0,0 = black
     */
    Vector3d GetEmissivityColor() const;
    /**
     * @return Amount of light this object reflects. 0.0 = none, 1.0 = mirror
     */
    double GetReflectivity() const;
    /**
     * @return object type. Used for type differentiation in Scene.Render(). 
     */
    virtual Object_Type GetType() const = 0;

private:
    std::string name_;
    Vector3d pos_;
    Vector3d color_;
    Vector3d emissivity_;
    double reflectivity_;
};

#endif