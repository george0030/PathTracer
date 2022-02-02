#pragma once

#include "object.hpp"
#include "sphere_object.hpp"
#include "mesh_object.hpp"
#include <vector>
#include "SFML/Graphics.hpp"


//24 bit RGB color
struct Color{
    uint8_t red_;
    uint8_t green_;
    uint8_t blue_;
    Color(uint8_t red, uint8_t green, uint8_t blue) : red_(red), green_(green), blue_(blue)
    {}
};


class Scene {
public:
    Scene() {}
    ~Scene();
    /**
     * @brief Add an object to this scene
     * 
     * @param obj The object to be added
     */
    void Add(Object* obj);
    /**
     * @brief Calculate the distance from the origin with the specified ray direction to the sphere object.
     * 
     * @param rayorig Ray origin
     * @param raydir Ray direction
     * @param sphere Sphere object
     * @return Distance from the ray origin to the object. If the ray does not intersect, -1 is returned.
     */
    double IntersectionWithSphere(Vector3d rayorig, Vector3d raydir, Sphere* sphere);/**
    /**
     * @brief Calculate the distance from the origin with the specified ray direction to a triangular face.
     * 
     * @param rayorig Ray origin
     * @param raydir Ray direction
     * @param mesh Mesh object that the triangle is a part of
     * @param v0 First face-defining vertex
     * @param v1 Second face-defining vertex
     * @param v2 Third face-defining vertex
     * @param normal Normal vector of the face
     * @return Distance from the ray origin to the face. If the ray does not intersect, -1 is returned.
     */
    double IntersectionWithFace(Vector3d rayorig, Vector3d raydir, Mesh* mesh, const Vector3d &v0, const Vector3d &v1, const Vector3d &v2, Vector3d &normal);

    /**
     * @brief Trace a ray from rayorig to raydir.
     * @param rayorig Origin of the ray
     * @param raydir Direction of the ray
     * @param depth Maximum number of bounces for the ray
     * @return Vector3d 
     */
    Vector3d TraceRay(Vector3d rayorig, Vector3d raydir, unsigned int depth);
    /**
     * @brief Render a picture from this scene, according to camera settings.
     * To speed up the rendering process, OpenMP will be used to parallelize the rendering process.
     * @param image SFML image where to write the rendered scene.
     */
    void Render(sf::Image& image);
    /**
     * @brief Set camera location
     * 
     * @param pos Camera location
     */
    void setCameraPos(Vector3d pos);
    /**
     * @brief Set camera rotation
     * 
     * @param rotation Camera rotation
     */
    void setCameraRotation(Quaterniond rotation);
    /**
     * @brief Set the rendering resolution width
     * 
     * @param cameraResolutionWidth Width in pixels
     */
    void setCameraResolutionWidth(unsigned int cameraResolutionWidth);
    /**
     * @brief Set the rendering resolution height
     * 
     * @param cameraResolutionHeight Height in pixels
     */
    void setCameraResolutionHeight(unsigned int cameraResolutionHeight);
    /**
     * @brief Set the rendering horizontal field of view
     * 
     * @param fov Horizontal field of view, in degrees
     */
    void setCameraFov(double fov);

    /**
     * @brief Set the maximum number of bounces for TraceRay()
     * @param maxRecursionDepth Maximum recursion depth
     */
    void setMaxRecursionDepth(unsigned int maxRecursionDepth);
    /**
     * @brief Set the background color
     * 
     * @param color Background color
     */
    void setBackgroundColor(Vector3d color);
    /**
     * @brief Get rendering resolution width
     * 
     * @return Width in pixels
     */
    unsigned int getCameraResolutionWidth() const;
    /**
     * @brief Set the maximum number of bounces for TraceRay()
     * 
     * @return Maximum recursion depth
     */
    unsigned int getCameraResolutionHeight() const;

    /**
     * @brief Get the Max Recursion Depth object
     * 
     * @return unsigned int 
     */
    unsigned int getMaxRecursionDepth() const;

    /**
     * @brief Get background color
     * @return Background color
     */
    Vector3d getBackgroundColor() const;
    /**
     * @brief Get all objects that have been added to the secene
     * 
     * @return Vector of objects in the scene.
     */
    const std::vector<Object*>& getObjects();

    /**
     * @brief Get an object in the scene by name
     * @param name Name of the object
     * @return An object by the name name. If the object is not found, nullptr is returned.
     */
    Object* const getObjectByName(const std::string& name);

    /**
     * @brief Delete an object in the scene by name.
     * @param name The name of the object
     * @return true The object was found and deleted successfully.
     * @return false The object was not found and could not be deleted.
     */
    bool deleteObjectByName(const std::string& name);

private:
    std::vector<Object*> objects_;
    Vector3d camera_pos_;
    Quaterniond camera_rotation_;
    unsigned int camera_resolution_width_;
    unsigned int camera_resolution_height_;
    double camera_fov_;
    Vector3d background_color_;
    unsigned int max_recursion_depth_;
};