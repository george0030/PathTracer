#ifndef MESH_CLASS
#define MESH_CLASS

#include "object.hpp"

#include <vector>

/**
 * @brief Represents a single triangular face in a triangle mesh object.
 */
struct Face {
    /** Index of the first vertex of the face. Get the vertex location with 
     * @see GetVertex()
     */
    unsigned int vertexA; 
    /** Index of the second vertex of the face. Get the vertex location with 
     * @see GetVertex()
     */
    unsigned int vertexB; 
     /** Index of the third vertex of the face. Get the vertex location with 
      * @see GetVertex()
      */
    unsigned int vertexC;
};
/**
 * Represents a 3d triangle mesh object.
 */
class Mesh : public Object {
public:
    Mesh(const std::string& name, Vector3d pos, Vector3d color, Vector3d emissivity, double reflectivity);
    /**
     * @brief Add a vertex to the mesh.
     * 
     * @param relativeLocation Relative location of the vertex from the mesh origin (0,0,0)
     * @return unsigned int 
     */
    unsigned int AddVertex(Vector3d relativeLocation);
    /**
     * @brief Add a face to the triangle mesh
     * 
     * @param face Face (collection of 3 vertices by index)
     * @return Face index
     */
    unsigned int AddFace(Face face); /** Add a face to the mesh. */
    /**
     * @brief Get the relative vertex location by index
     * 
     * @param index Vertex index
     * @return Vector3d Relative location of the vertex from the mesh origin (0,0,0)
     */
    Vector3d GetVertex(unsigned int index) const; 
    /**
     * @brief Get all faces that this triangle mesh contains
     * 
     * @return Collection of faces by vertex index
     */
    const std::vector<Face>& GetFaces();  
    Object_Type GetType() const { return MESH;}

private:
    std::vector<Vector3d> vertices_;
    std::vector<Face> faces_;
};

#endif