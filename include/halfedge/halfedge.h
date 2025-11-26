#ifndef AUTOFRAC_HE_HALFEDGE_H
#define AUTOFRAC_HE_HALFEDGE_H

#include <QString>

namespace he {

class Vertex;

class Face;

class HalfEdge {
public:
    /**
     * @brief Construct a half-edge with its origin
     * @param origin the origin of the half-edge
     */
    explicit HalfEdge(he::Vertex* origin, QString name = "");

    /**
     * @brief getter
     * @return the origin of the half-edge
     */
    he::Vertex* origin();

    /**
     * @brief setter
     * @param origin the origin to set for this half-edge
     */
    void setOrigin(he::Vertex* origin);

    /**
     * @brief getter
     * @return the face of this half-edge
     */
    he::Face* face() const;

    /**
     * @brief setter
     * @param face the face to set for this half-edge
     */
    void setFace(he::Face* face);

    /**
     * @brief getter
     * @return the twin half-edge of this half-edge
     */
    HalfEdge* twin() const;

    /**
     * @brief setter
     * @param twin the twin half-edge to be set for this half-edge
     */
    void setTwin(HalfEdge* twin);

    /**
     * @brief getter
     * @return the previous half-edge of this half-edge
     */
    HalfEdge* prev();

    /**
     * @brief setter
     * @param prev the previous half-edge to be set for this half-edge
     */
    void setPrev(HalfEdge* prev);

    /**
     * @brief getter
     * @return the next half-edge of this half-edge
     */
    HalfEdge* next();

    /**
     * @brief setter
     * @param next the next half-edge to be set for this half-edge
     */
    void setNext(HalfEdge* next);

    /**
     * @brief getter
     * @return the name of this half-edge, represented by the vertices
     * it bind, in the order origin -> next.origin
     */
    QString name() const;

    /**
     * @brief setter
     * @param name the name to be set for this half-edge
     */
    void setName(QString const& name);

    /**
     * getter
     * @return the length of the edge
     */
    float length() const;

    /**
     * getter
     * @return the string version of this edge (mainly for debugging purpose)
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

private:
    he::Vertex* m_origin;
    he::Face* m_face;
    HalfEdge* m_twin;
    HalfEdge* m_prev;
    HalfEdge* m_next;
    QString m_name;
    QString m_userData;
};

} // poly

#endif //AUTOFRAC_HE_HALFEDGE_H
