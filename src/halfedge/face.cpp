#include <vector>
#include <QMatrix4x4>

#include "halfedge/face.h"
#include "halfedge/halfedge.h"
#include "halfedge/vertex.h"

he::Face::Face(QString name, he::HalfEdge* halfEdge) : m_name(std::move(name)), m_halfEdge(halfEdge) {}

he::HalfEdge* he::Face::halfEdge() const {
    return m_halfEdge;
}

void he::Face::setHalfEdge(he::HalfEdge* halfEdge) {
    m_halfEdge = halfEdge;
}

QString he::Face::name() const {
    return m_name;
}

QVector3D he::Face::computeNormal() const {
    // Newell's method to compute the normal of any convex or concave polygon
    // https://wikis.khronos.org/opengl/Calculating_a_Surface_Normal
    std::vector<he::Vertex*> vertices = this->allVertices();
    QVector3D normal = { 0, 0, 0 };
    for (std::size_t i = 0; i < vertices.size(); i++) {
        const QVector3D& a = vertices[i]->pos();
        const QVector3D& b = vertices[(i + 1) % vertices.size()]->pos();
        normal.setX(normal.x() + (a.y() - b.y()) * (a.z() + b.z()));
        normal.setY(normal.y() + (a.z() - b.z()) * (a.x() + b.x()));
        normal.setZ(normal.z() + (a.x() - b.x()) * (a.y() + b.y()));
    }
    normal.normalize();
    return normal;
}

he::Point3D he::Face::computeNormalD() const {
    // Newell's method to compute the normal of any convex or concave polygon
    std::vector<he::Vertex*> vertices = this->allVertices();
    he::Point3D normal = { 0, 0, 0 };
    for (std::size_t i = 0; i < vertices.size(); i++) {
        const he::Point3D& a = vertices[i]->posD();
        const he::Point3D& b = vertices[(i + 1) % vertices.size()]->posD();
        normal.setX(normal.x() + (a.y() - b.y()) * (a.z() + b.z()));
        normal.setY(normal.y() + (a.z() - b.z()) * (a.x() + b.x()));
        normal.setZ(normal.z() + (a.x() - b.x()) * (a.y() + b.y()));
    }
    normal.normalize();
    return normal;
}

std::size_t he::Face::nbEdges() const {
    std::size_t res = 0;

    he::HalfEdge* he = this->m_halfEdge;
    he::HalfEdge* heNxt = he;
    do {
        res++;
        heNxt = heNxt->next();
    } while (heNxt != he);

    return res;
}

std::vector<he::HalfEdge*> he::Face::allHalfEdges() const {
    std::vector<he::HalfEdge*> res;

    he::HalfEdge* he = this->m_halfEdge;
    he::HalfEdge* heNxt = he;
    do {
        res.push_back(heNxt);
        heNxt = heNxt->next();
    } while (heNxt != he);

    return res;
}

std::vector<he::Vertex*> he::Face::allVertices() const {
    std::vector<he::Vertex*> res;

    for (he::HalfEdge* he: this->allHalfEdges()) {
        res.push_back(he->origin());
    }

    return res;
}

float he::Face::area() const {
    float res = 0.0f;
    he::HalfEdge* he = this->m_halfEdge;
    he::HalfEdge* heNxt = he;
    QMatrix4x4 invTransMat = this->basisChangeMatrix();
    do {
        QVector4D p1 = invTransMat * QVector4D(heNxt->origin()->pos(), 1.0f);
        QVector4D p2 = invTransMat * QVector4D(heNxt->next()->origin()->pos(), 1.0f);
        res += p1.x() * p2.y() - p2.x() * p1.y();
        heNxt = heNxt->next();
    } while (heNxt != he);

    return res / 2.0f;
}

QVector3D he::Face::barycenter() const {
    QVector3D res(0.0f, 0.0f, 0.0f);
    std::vector<he::HalfEdge*> halfedges = this->allHalfEdges();
    for (he::HalfEdge* he: halfedges) {
        res += he->origin()->pos();
    }
    res /= static_cast<float>(halfedges.size());
    return res;
}

QString he::Face::toString() const {
    QString res;
    res += m_name + ": has the halfedge " + (m_halfEdge ? m_halfEdge->name() : "nullptr") + " User data: " + m_userData;
    return res;
}

QString he::Face::userData() const {
    return m_userData;
}

void he::Face::setUserData(QString const& data) {
    m_userData = data;
}

he::Point3D he::Face::computePolar() const {
    // polar reciprocation : https://en.wikipedia.org/wiki/Dual_polyhedron#Polar_reciprocation
    he::Point3D n = this->computeNormalD();
    he::Point3D p = this->halfEdge()->origin()->posD();
    double d = he::Point3D::dotProduct(n, p);
    n /= d;
    return n;
}

QMatrix4x4 he::Face::basisChangeMatrix() const {
    he::Vertex* v0 = m_halfEdge->origin();
    he::Vertex* v1 = m_halfEdge->next()->origin();
    QVector3D axisX = (v1->pos() - v0->pos()).normalized();

    QVector3D axisZ = this->computeNormal();

    QVector3D axisY = QVector3D::crossProduct(axisZ, axisX);

    // canonic basis B = { (1,0,0),  (0,1,0),  (0,0,1) }    and     B' = { axisX, axisY, axisZ }
    QMatrix4x4 P {
            axisX.x(), axisY.x(), axisZ.x(), 0,
            axisX.y(), axisY.y(), axisZ.y(), 0,
            axisX.z(), axisY.z(), axisZ.z(), 0,
            0, 0, 0, 1
    };
    QMatrix4x4 pInv = P.inverted();
    return pInv;
}
