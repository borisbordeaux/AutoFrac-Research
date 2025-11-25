#ifndef AUTOFRAC_BATCHFACE_H
#define AUTOFRAC_BATCHFACE_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "batchgraphicsitem.h"

namespace he {
class Mesh;

class Face;

class Vertex;
}

class BatchFace : public BatchGraphicsItem {
public:
    void init() override;
    void update() override;
    void updateData() override;
    void render(PickingType type) override;

    void setProjection(QMatrix4x4 projection) override;
    void setCamera(Camera camera) override;
    void setLight(QVector3D lightPos) override;
    void setColor(QColor const& color) override;

    void setMesh(he::Mesh* mesh);

    void setSelectedFace(int faceIndex);
    he::Face* selectedFace() const;

    int renderOrder() override;
    int pickingOrder() override;

private:
    static qsizetype findNbOfTriangle(he::Mesh const* mesh);
    void addFace(he::Face* f, int ID);
    static bool isValidTriangle(he::Vertex const* prev, he::Vertex const* current, he::Vertex const* next, he::Face const* face, QMatrix4x4 const& projMatrix);
    static float cross2D(QVector2D const& u, QVector2D const& v);
    static bool pointInTriangle(QVector2D const& P, QVector2D const& A, QVector2D const& B, QVector2D const& C);
    void triangle(QVector3D const& pos1, QVector3D const& pos2, QVector3D const& pos3, float ID, float isSelected);
    void addVertexFace(QVector3D const& v, QVector3D const& n, float ID, float isSelected);

private:
    QVector<float> m_data;
    int m_count = 0;
    int m_floatsPerVertex = 8;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram m_program;
    QOpenGLShaderProgram m_programPicking;

    //Faces viewing
    int m_projMatrixLoc = 0;
    int m_viewMatrixLoc = 0;
    int m_lightPosLoc = 0;
    int m_cameraPosLoc = 0;
    int m_colorLoc = 0;
    //Faces picking
    int m_projMatrixPickingLoc = 0;
    int m_viewMatrixPickingLoc = 0;

    he::Mesh* m_mesh = nullptr;
    int m_selectedFace = 0;
};


#endif //AUTOFRAC_BATCHFACE_H
