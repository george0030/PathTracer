// sceneparser.hpp
#pragma once

#include <string>

#include "rapidxml.hpp"

#include "scene.hpp"
#include "sphere_object.hpp"
#include "plane_object.hpp"
#include "mesh_object.hpp"
/**
 * @defgroup SceneParser XML scene parser.
 * @{
 */


using namespace rapidxml;
/**
 * @brief Parses a Scene object from an XML file. See doc/scene_sample.xml
 * @param filePath File location of the scene file.
 * @param scene Reference to the scene which will be configured according to the xml.
 * @return The scene
 */
void parseSceneFromFile(const std::string& filePath, Scene& scene);
/**
 * @brief Parses a sphere object from XML
 * @param node XML node containing the sphere object
 * @return The sphere object
 */
Sphere* parseSphere(xml_node<>* node);
/**
 * @brief Parses a location object (3-dimensional vector) from XML
 * @param node XML node containing the location object
 * @return The location object
 */
Vector3d parseLocation(xml_node<>* node);
/**
 * @brief Parses a color object (3-dimensional vector), x=red, y=green, z=blue from XML
 * @param node XML node containing the color object
 * @return The location object
 */
Vector3d parseColor(xml_node<>* node);
/**
 * @brief Parses a plane object from XML
 * @param node XML node containing the plane object
 * @return The location object
 */
Plane* parsePlane(xml_node<>* node);
/**
 * @brief Adds a 3d triangle mesh object from XML
 * @param node XML node containing the 3d triangle mesh object
 * @param filePath File path of the XML node. Given to determine .obj file location.
 * @return The triangle mesh object
 */
Mesh* parseMesh(xml_node<>* node, const std::string& filePath);
/**
 * @brief Parses a mesh object from an .obj file. Only faces and vertices are supported
 * 
 * @param mesh The mesh object the vertices and faces are added to
 * @param objFile Location of the .obj file.
 */
void addFacesAndVertices(Mesh* mesh, const std::string& objFile);
/** @} */
