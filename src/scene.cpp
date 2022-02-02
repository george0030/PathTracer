#include "scene.hpp"
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>

#include <omp.h>
#include <Eigen/Geometry>

constexpr float kEpsilon = 1e-8; 

    Scene::~Scene(){
        for (auto o: objects_){
            delete o;
        }
    }
    void Scene::Add(Object* obj) {
        objects_.push_back(obj);
    }
    void Scene::setCameraPos(Vector3d pos) {
        camera_pos_ = pos;
    }
    void Scene::setCameraRotation(Quaterniond rotation){
        camera_rotation_ = rotation;
    }
    void Scene::setCameraResolutionWidth(unsigned int cameraResolutionWidth){
        camera_resolution_width_ = cameraResolutionWidth;
    }
    void Scene::setCameraResolutionHeight(unsigned int cameraResolutionHeight){
        camera_resolution_height_ = cameraResolutionHeight;
    }
    void Scene::setCameraFov(double fov){
        camera_fov_ = fov;
    }

    void Scene::setBackgroundColor(Vector3d color) {
        background_color_ = color;
    }

    void Scene::setMaxRecursionDepth(unsigned int maxRecursionDepth){
        max_recursion_depth_ = maxRecursionDepth;
    }
    unsigned int Scene::getMaxRecursionDepth() const {
        return max_recursion_depth_;
    }
    unsigned int Scene::getCameraResolutionWidth() const {
        return camera_resolution_width_;
    }
    unsigned int Scene::getCameraResolutionHeight() const {
        return camera_resolution_height_;
    }
    Vector3d Scene::getBackgroundColor() const {
        return background_color_;
    }


    const std::vector<Object*>& Scene::getObjects(){
        return objects_;
    }

    Object* const Scene::getObjectByName(const std::string& name){
        for(auto o : objects_){
            if(name.compare(o->GetName()) == 0){
                return o;
            }
        }
        return nullptr;
    }

    bool Scene::deleteObjectByName(const std::string& name){
        for(auto o : objects_){
            if(name.compare(o->GetName()) == 0){
                delete o;
                return true;
            }
        }
        return false;
    }

    double Scene::IntersectionWithSphere(Vector3d rayorig, Vector3d raydir, Sphere* sphere){
        // find distance t to a intersection point and return it, if there is none, return -1.0
        double r = sphere->GetRadius();
        Vector3d p = sphere->GetPosition();
        Vector3d oc = rayorig - p;
        double a = raydir.dot(raydir);
        double b = 2.0 * oc.dot(raydir);
        double c = oc.dot(oc) - r*r;
        double discriminant = b*b - 4*a*c;
        //std::cout << discriminant ;
        if(discriminant < 0.0){
            return -1.0;
        }
        
        float numerator = -b - sqrt(discriminant);
        if (numerator > 0.0) {
            return numerator / (2.0 * a);
        }

        numerator = -b + sqrt(discriminant);
        if (numerator > 0.0) {
            return numerator / (2.0 * a);
        } else {
            return -1.0;
        }

    }

    double Scene::IntersectionWithFace(Vector3d rayorig, Vector3d raydir, Mesh* mesh,
        const Vector3d &v0, const Vector3d &v1, const Vector3d &v2, Vector3d &normal) {
        
        //Compute normal
        Vector3d v0v1 = v1 - v0;
        Vector3d v0v2 = v2 - v0;

        normal = v0v1.cross(v0v2);
        //Moller Trumbore magic

        Vector3d pVec = raydir.cross(v0v2);
        double det = v0v1.dot(pVec);

        if (fabs(det) < kEpsilon) return -1.0;

        double invdet = 1 / det;

        Vector3d tvec = rayorig - v0;

        double u = tvec.dot(pVec) * invdet;

        if (u < 0 || u > 1) return -1.0;

        Vector3d qvec = tvec.cross(v0v1);
        double v = raydir.dot(qvec) * invdet;
        if (v < 0 || u + v > 1) return -1.0;

        // distance t
        return v0v2.dot(qvec) * invdet;
    }   

    

    Vector3d Scene::TraceRay(Vector3d rayorig, Vector3d raydir, unsigned int depth) {
        //traces a ray from pixel and tests if it intersects with any of the objects in the scene

        //all objects_ in the scene
        double t_closest = 100000000.0; // closest distance to intersection point
        Object* obj = NULL;

        Sphere* currentSphere = NULL;
        Mesh * currentMesh = NULL;

        Object* current = NULL;

        Vector3d normal;

        for (unsigned i = 0; i < objects_.size(); ++i) {

           //check if intersect is sphere
            if(objects_[i]->GetType() == SPHERE){

                currentSphere = (Sphere *) objects_[i]; 
                //find out if the ray intersects with any objects by going through one object at a time
                //if we find only one object, save that to obj
                //if we find multiple, save the closest object to obj and closest distance t to t_closest
                double t = IntersectionWithSphere(rayorig, raydir, currentSphere);
                if (t != -1.0 && t < t_closest) {                        
                    t_closest = t;
                    obj = currentSphere;
                }
            }

            // check if intersect mesh
            if(objects_[i]->GetType() == MESH) {

                currentMesh = (Mesh*) objects_[i];

                double t = -1.0;
                
                for(int j = 0; j < currentMesh->GetFaces().size();  j ++){
                    t = IntersectionWithFace(rayorig, raydir, currentMesh,
                        currentMesh->GetVertex(currentMesh->GetFaces().at(j).vertexA) + currentMesh->GetPosition(), currentMesh->GetVertex(currentMesh->GetFaces().at(j).vertexB)+ currentMesh->GetPosition(), currentMesh->GetVertex(currentMesh->GetFaces().at(j).vertexC)+ currentMesh->GetPosition(), normal);
                    if(t != -1.0 && t < t_closest)  break;
                }
                if(t != -1.0 && t < t_closest) {
                    t_closest = t;
                    obj = objects_[i];
                }

            }
        } 

        // if we find no intersecting objects, return background color for this ray
        if (!obj) {
            return background_color_; 
        }


        Vector3d hitPoint; // intersection point
        Vector3d hitNormal; // normal at the intersection point 
        
        Vector3d surfaceColor = 0.1 * obj->GetColor(); //ambient light
        //IF hit mesh, calculate variables here
        if (obj->GetType() == MESH) {

            // Calculate phit, nhit and other variables for reflection
            // Return color if not reflective

            if (obj->GetReflectivity() == 0.0) return obj->GetColor(); 
            hitPoint = rayorig + raydir * t_closest; // intersection point
            hitNormal = normal;
            hitNormal.normalize(); // normalized normal vector

        } else { // not mesh, sphere
            hitPoint = rayorig + raydir * t_closest; // intersection point
            hitNormal = hitPoint - obj->GetPosition(); // normal at the intersection point 
            hitNormal.normalize(); // normalized normal vector
        }


        
        //object has some sort of reflectivity
        if (obj->GetReflectivity() > 0 && depth < max_recursion_depth_) { 
            double facingRatio = -raydir.dot(hitNormal); 
            double fresnel = pow(1 - facingRatio, 3) * 0.9 + 1.0 * 0.1; // fresnel effect
            Vector3d reflectionDirection = raydir - hitNormal * 2 * raydir.dot(hitNormal); //reflection direction
            reflectionDirection.normalize(); 
            Vector3d reflection = TraceRay(hitPoint + hitNormal, reflectionDirection, depth + 1);
            Vector3d objectColor = obj->GetColor();
            Vector3d addedColor = Vector3d(objectColor.x() * reflection.x(), objectColor.y() * reflection.y(), objectColor.z() * reflection.z());
            surfaceColor += addedColor * fresnel;
        } 
        else { 
            if (obj->GetEmissivityColor().x() > 0.0) { //check if object itself is a light
                return obj->GetColor();
            }
            // there is no reflections for this object or max recursion depth is reached
            for (unsigned i = 0; i < objects_.size(); ++i) { 
                if (objects_[i]->GetEmissivityColor().x() > 0.0) { 
                    // finally trace ray from object to light 
                    Vector3d lightDirection = objects_[i]->GetPosition() - hitPoint; 
                    lightDirection.normalize();
                    for (unsigned j = 0; j < objects_.size() && j != i; ++j) { 
                        if(objects_[j]->GetType() == SPHERE){
                            Sphere* sphere2 = (Sphere*) objects_[j];
                            if (IntersectionWithSphere(hitPoint + hitNormal, lightDirection, sphere2) != -1.0) {
                                // some other object is in the way of light source -> creates shadow
                                return Vector3d(0.0, 0.0, 0.0); //return black
                            }
                        } 
                    } 
                    double diffuseShading = std::max(double(0), hitNormal.dot(lightDirection));
                    Vector3d light = objects_[i]->GetEmissivityColor();
                    Vector3d objectColor = obj->GetColor();
                    Vector3d addedColor = Vector3d(objectColor.x() * light.x(), objectColor.y() * light.y(), objectColor.z() * light.z());
                    surfaceColor += addedColor * diffuseShading;
                } 
            } 
        } 
        return surfaceColor;
    }


    void Scene::Render(sf::Image& image) {
        
        unsigned int width = camera_resolution_width_, height = camera_resolution_height_;
        image.create(width, height);
        double invWidth = 1 / double(width);
        double invHeight = 1 / double(height); 
        double fov = camera_fov_;
        double aspectRatio = width / double(height); 
        double angle = tan(M_PI * 0.5 * fov / 180.); 
        
        int t0 = time(NULL);
        unsigned int rowsRendered = 0;
        #pragma omp parallel
        {
            #pragma omp single
            {
                std::cout << "Rendering with " << omp_get_num_threads() << " thread(s)." << std::endl;
            }
            #pragma omp for nowait
            for (unsigned y = 0; y < height; ++y) {
                #pragma omp critical
                {
                    int done = (int)(100.0 * ( (1.0 *  rowsRendered ) / ( 1.0 * height ) ));
                    std::cout << "Done: " << done << "%        \r";
                }
                for (unsigned x = 0; x < width; ++x) { 
                    double xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectRatio; 
                    double yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle; 
                    Vector3d raydir(xx, yy, -1); 
                    raydir.normalize(); 
                    Vector3d pixelColor = TraceRay(camera_pos_, raydir, 0);
                    Vector3d pixelColorFixed = Vector3d(std::min(double(1), pixelColor.x()), std::min(double(1), pixelColor.y()), std::min(double(1), pixelColor.z()));
                    uint8_t red = (uint8_t) (pixelColorFixed.x()*255.0);
                    uint8_t green = (uint8_t) (pixelColorFixed.y()*255.0);
                    uint8_t blue = (uint8_t) (pixelColorFixed.z()*255.0);
                    image.setPixel(x,y, sf::Color(red, green, blue));
                }
                #pragma omp atomic
                rowsRendered++; 
            }
        }
        std::cout << std::endl << " Time render took: " << time(NULL) - t0 << " s" << std::endl;
    }
