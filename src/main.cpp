
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <chrono>

#include <thread>

#include "SFML/Graphics.hpp"
#include "sceneparser.hpp"
#include "object.hpp"
#include "sphere_object.hpp"
#include <vector>

#include "sceneparser.hpp"
#include "scene.hpp"

int main(int argc, char *argv[]) 
{

    if(argc < 2){
        std::cout << "Usage: PathTracer <scene_file> <out_file>" << std::endl;
        return 1;
    }

    std::string filenamein(argv[1]);
    std::string filenameout(argv[2]);
    Scene scene;
    
    unsigned int filenameoutPeriod = filenameout.find_first_of('.');
    if(filenameoutPeriod == -1){
        std::cerr << "Unsupported type for output. Only types .bmp, .png, .tga and .jpg are supported." << std::endl;
        return 1;
    }
    std::string type = filenameout.substr(filenameoutPeriod);
    if((type.compare(".bmp") != 0) &&
       (type.compare(".png") != 0) &&
       (type.compare(".tga") != 0) &&
       (type.compare(".jpg") != 0)){
        std::cerr << "Unsupported type for output. Only types .bmp, .png, .tga and .jpg are supported." << std::endl;
        return 1;
    }

    try{
        parseSceneFromFile(filenamein, scene);
    }
    catch(const std::exception& e){
        std::cerr << "Failed to read scene file: " << filenamein << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
    std::cout << "Scene loaded from " << filenamein << ". Scene contains the following objects:" << std::endl << std::endl;
    for(auto o : scene.getObjects()){
        std::string name = o->GetName();
        Object_Type type = o->GetType();
        Vector3d position = o->GetPosition();
        Vector3d color = o->GetColor();
        Vector3d emissivity = o->GetEmissivityColor();
        double specularity = o->GetReflectivity();
        std::cout << name << std::endl
        << "Location: x: " << position.x() << ", y: " << position.y() << ", z: " << position.z() << std::endl
        << "Color: red: " << color.x() << ", green: " << color.y() << ", blue: " << color.z() << std::endl
        << "Emissivity: red: " << emissivity.x() << ", green: " << emissivity.y() << ", blue: " << emissivity.z() << std::endl
        << "Specularity: " << specularity << std::endl << std::endl;

    }
    Vector3d bgColor = scene.getBackgroundColor();
    std::cout << "Background color set to: red:" << bgColor.x() << ", green: " << bgColor.y() << ", blue: " << bgColor.z() << std::endl;
    std::cout << "Camera resolution set to: Width: " << scene.getCameraResolutionWidth() << ", Height: " << scene.getCameraResolutionHeight() << std::endl;
    std::cout << "Maximum recursion depth: " << scene.getMaxRecursionDepth() << std::endl << std::endl;
    std::cout << "Beginning to render scene. Rendered scene will be written to " << filenameout << std::endl << std::endl;
    sf::Image image;
    scene.Render(image);
    image.saveToFile(filenameout);
    return 0;
    
}