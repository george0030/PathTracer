// sceneparser.cpp

#include "sceneparser.hpp"
#include <fstream>

using namespace rapidxml;

inline char separator(){
    #ifdef _WIN32 
        return '\\';
    #else
        return '/';
    #endif
}

void parseSceneFromFile(const std::string& filePath, Scene& scene) {

    xml_document<> doc;
    std::ifstream in(filePath);
    std::string contents((std::istreambuf_iterator<char>(in)), 
    std::istreambuf_iterator<char>());
    char* contentsCopied = new char[contents.size()];
    contents.copy(contentsCopied, contents.size(), 0);
    doc.parse<0>(contentsCopied);
    xml_node<>* sceneNode = doc.first_node("scene");
    xml_node<>* objectsNode = sceneNode->first_node("objects");
            for(xml_node<>* objectNode = objectsNode->first_node(); objectNode; objectNode = objectNode->next_sibling()){
                char* name = objectNode->name();
                if(strcmp(name, "sphere") == 0){
                    scene.Add(parseSphere(objectNode));
                }
                else if(strcmp(name, "plane") == 0){
                    scene.Add(parsePlane(objectNode));
                }
                else if(strcmp(name, "mesh") == 0){
                    scene.Add(parseMesh(objectNode, filePath));
                }
            }
    xml_node<>* cameraNode = sceneNode->first_node("camera");
        for(xml_node<>* next = cameraNode->first_node(); next; next = next->next_sibling()){
            char* name = next->name();
            if(strcmp(name, "location") == 0){
                scene.setCameraPos(parseLocation(next));
            }
            else if(strcmp(name, "rotation") == 0){
                //scene.setCameraRotation(parseRotation(next));
            }
            else if(strcmp(name, "width") == 0){
                scene.setCameraResolutionWidth(strtol(next->value(), NULL, 10));
            }
            else if(strcmp(name, "height") == 0){
                scene.setCameraResolutionHeight(strtol(next->value(), NULL, 10));
            }
            else if(strcmp(name, "fov") == 0){
                scene.setCameraFov(atof(next->value()));
            }
            else if(strcmp(name, "maxRecursionDepth") == 0){
                scene.setMaxRecursionDepth(strtol(next->value(), NULL, 10));
            }
        }
    xml_node<>* backgroundColorNode = sceneNode->first_node("backgroundColor");
        scene.setBackgroundColor(parseColor(backgroundColorNode));
    }

Sphere* parseSphere(xml_node<>* node) {
    std::string objectName;
    Vector3d location(0,0,0);
    double radius = 0;
    Vector3d emissivity(0,0,0);
    Vector3d color(0,0,0);
    //used as reflectivity when constructing, might change these names later
    double specularity = 0;
    //unused
    double roughness = 0;
    for(xml_node<>* next = node->first_node(); next; next = next->next_sibling()){
        char* name = next->name();
        if(strcmp(name, "name") == 0){
            objectName = std::string(next->value());
        }
        else if(strcmp(name, "location") == 0){
            location = parseLocation(next);
        }
        else if(strcmp(name, "radius") == 0){
            radius = atof(next->value());
        }
        else if(strcmp(name, "emissivity") == 0){
            emissivity = parseColor(next);
        }
        else if(strcmp(name, "color") == 0){
            color = parseColor(next);
        }
        else if(strcmp(name, "specularity") == 0){
            specularity = atof(next->value());
        }
        else if(strcmp(name, "roughness") == 0){
            roughness = atof(next->value());
        }
    }
    
    return new Sphere(objectName, radius, location, color, emissivity, specularity);
    
}

Vector3d parseLocation(xml_node<>* node){
    double x = 0;
    double y = 0;
    double z = 0;

    for(xml_node<>* next = node->first_node(); next; next = next->next_sibling()){
        char* name = next->name();
        if(strcmp(name, "x") == 0){
            x = atof(next->value());
        }
        else if(strcmp(name, "y") == 0){
            y = atof(next->value());
        }
        else if(strcmp(name, "z") == 0){
            z = atof(next->value());
        }
    }
    return Vector3d(x,y,z);
}

Plane* parsePlane(xml_node<>* node){
    std::string objectName;
    Vector3d location(0,0,0);
    Vector3d normal(0,0,0);
    Vector3d emissivity(0,0,0);
    Vector3d color(0,0,0);
    //used as reflectivity when constructing, might change these names later
    double specularity;
    //unused
    double roughness;
    for(xml_node<>* next = node->first_node(); next; next = next->next_sibling()){
        char* name = next->name();
        if(strcmp(name, "name") == 0){
            objectName = std::string(next->value());
        }
        else if(strcmp(name, "location") == 0){
            location = parseLocation(next);
        }
        else if(strcmp(name, "normal") == 0){
            normal = parseLocation(next);
        }
        else if(strcmp(name, "emissivity") == 0){
            emissivity = parseColor(next);
        }
        else if(strcmp(name, "color") == 0){
            color = parseColor(next);
        }
        else if(strcmp(name, "specularity") == 0){
            specularity = atof(next->value());
        }
        else if(strcmp(name, "roughness") == 0){
            roughness = atof(next->value());
        }
    }
    
    return new Plane(objectName, location, normal, color, emissivity, specularity);

}


Vector3d parseColor(xml_node<>* node){
    double red = 0;
    double green = 0;
    double blue = 0;
    for(xml_node<>* next = node->first_node(); next; next = next->next_sibling()){
        char* name = next->name();
        if(strcmp(name, "red") == 0){
            red = atof(next->value());
        }
        else if(strcmp(name, "green") == 0){
            green = atof(next->value());
        }
        else if(strcmp(name, "blue") == 0){
            blue = atof(next->value());
        }
    }

    return Vector3d(red,green,blue);
}
Quaterniond parseRotation(xml_node<>* node){
    double a;
    double b;
    double c;
    double d;

    for(xml_node<>* next = node->first_node(); next; next = next->next_sibling()){
        char* name = next->name();
        if(strcmp(name, "a") == 0){
            a = atof(next->value());
        }
        else if(strcmp(name, "b") == 0){
            b = atof(next->value());
        }
        else if(strcmp(name, "c") == 0){
            c = atof(next->value());
        }
        else if(strcmp(name, "d") == 0){
            d = atof(next->value());
        }
    }

    return Quaterniond(a,b,c,d);
}

Mesh* parseMesh(xml_node<>* node, const std::string& filePath) {
    std::string objectName;
    Vector3d location(0,0,0);
    double radius = 0;
    Vector3d emissivity(0,0,0);
    Vector3d color(0,0,0);
    //used as reflectivity when constructing, might change these names later
    double specularity = 0;
    //unused
    double roughness = 0;
    const char* objFileName;
    for(xml_node<>* next = node->first_node(); next; next = next->next_sibling()){
        char* name = next->name();
        if(strcmp(name, "name") == 0){
            objectName = std::string(next->value());
        }
        else if(strcmp(name, "location") == 0){
            location = parseLocation(next);
        }
        else if(strcmp(name, "objFile") == 0){
            objFileName = next->value();
        }
        else if(strcmp(name, "emissivity") == 0){
            emissivity = parseLocation(next);
        }
        else if(strcmp(name, "color") == 0){
            color = parseColor(next);
        }
        else if(strcmp(name, "specularity") == 0){
            specularity = atof(next->value());
        }
        else if(strcmp(name, "roughness") == 0){
            roughness = atof(next->value());
        }
    }
    std::size_t last_separator = filePath.find_last_of(separator());
    std::string dir = filePath.substr(0, last_separator + 1);
    std::string objFilePath = dir + objFileName;
    Mesh* mesh = new Mesh(objectName, location, color, emissivity, specularity);
    addFacesAndVertices(mesh, objFilePath);
    return mesh;
}
void addFacesAndVertices(Mesh* mesh, const std::string& objFile){
    std::ifstream infile(objFile);

    std::string line;
    while(std::getline(infile, line)){
        std::istringstream iss(line);
        std::string parameter;
        if(line.rfind("v", 0) == 0){
            unsigned int i = 0;
            double x;
            double y;
            double z;
            while(std::getline(iss, parameter, ' ')){
                if(parameter.compare("v") != 0){
                    if(i == 0){
                        x = atof(parameter.c_str());
                    }
                    else if(i == 1){
                        y = atof(parameter.c_str());
                    }
                    else if(i == 2){
                        z = atof(parameter.c_str());
                    }
                    i++;
                }
            }
            mesh->AddVertex(Vector3d(x,y,z));
        }
        else if(line.rfind("f", 0) == 0){
            unsigned int i = 0;
            double a;
            double b;
            double c;
                while(std::getline(iss, parameter, ' ')){
                    if(parameter.compare("f") != 0){
                        if(i == 0){
                            a = atoi(parameter.c_str());
                        }
                        else if(i == 1){
                            b = atoi(parameter.c_str());
                        }
                        else if(i == 2){
                            c = atoi(parameter.c_str());
                        }
                        i++;
                    }
                }
            
            Face face;
            face.vertexA = a;
            face.vertexB = b;
            face.vertexC = c;
            mesh->AddFace(face);
        }
    }
}
