#ifndef AUTOFRAC_HE_FACE_H
#define AUTOFRAC_HE_FACE_H

#include <QString>
#include <QVector3D>
#include <QMatrix4x4>
#include "point3d.h"

namespace he {

class HalfEdge;

class Vertex;

class Face {
public:
    /**
     * @brief Construct a Face with one half-edge
     * @param halfEdge the half-edge the face will use
     */
    explicit Face(QString name = "", he::HalfEdge* halfEdge = nullptr);

    /**
     * @brief getter
     * @return the half-edge associated to this Face
     */
    he::HalfEdge* halfEdge() const;

    /**
     * @brief setter
     * @param halfEdge the half-edge that has to be
     * associated to this Face
     */
    void setHalfEdge(he::HalfEdge* halfEdge);

    /**
     * @brief getter
     * @return the name of this face
     */
    QString name() const;

    /**
     * getter
	 * @return the normalized normal of the face
	 */
    QVector3D computeNormal() const;

    /**
	 * getter
	 * @return the normalized normal of the face with double precision
	 */
    he::Point3D computeNormalD() const;

    /**
     * getter
     * @return the number of edges of the face
     */
    std::size_t nbEdges() const;

    /**
     * getter
     * @return all halfedges belonging to this face
     */
    std::vector<he::HalfEdge*> allHalfEdges() const;

    /**
     * getter
     * @return all vertices belonging to this face
     */
    std::vector<he::Vertex*> allVertices() const;

    /**
     * getter
     * @return the area of the face
     */
    float area() const;

    /**
     * getter
     * @return the barycenter of this face
     */
    QVector3D barycenter() const;

    /**
     * getter
     * @return the string version of this face (mainly for debugging purpose)
     */
    QString toString() const;

    /**
     * getter
     * @return the userdata attached to this face
     */
    QString userData() const;

    /**
     * setter
     * @param data the new userdata to set for this face
     */
    void setUserData(QString const& data);

    /**
     * computes the polar reciprocation (https://en.wikipedia.org/wiki/Dual_polyhedron#Polar_reciprocation)
     * @return the position of the dual polyhedron vertex associated to this face
     */
    he::Point3D computePolar() const;

    /**
     * getter for the matrix to use in order to change the world basis with an orthonormal
     * basis such that any point in the same plane as this face has a zero z coordinate
     * @return the basis change matrix
     */
    QMatrix4x4 basisChangeMatrix() const;

private:
    QString m_name;
    he::HalfEdge* m_halfEdge;
    QString m_userData;
};

}

#endif //AUTOFRAC_HE_FACE_H
