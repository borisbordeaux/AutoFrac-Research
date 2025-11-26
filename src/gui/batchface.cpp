#include "gui/batchface.h"
#include "halfedge/mesh.h"
#include "halfedge/face.h"
#include "halfedge/halfedge.h"
#include "halfedge/vertex.h"
#include <QColor>

void BatchFace::init() {
    this->initializeOpenGLFunctions();

    m_vao.create();
    m_vbo.create();

    m_vao.bind();
    m_vbo.bind();

    //enable enough attrib array for all the data of the mesh's vertices
    glEnableVertexAttribArray(0); //coordinates
    glEnableVertexAttribArray(1); //normal
    glEnableVertexAttribArray(2); //ID for picking
    glEnableVertexAttribArray(3); //is selected
    //3 coordinates of the vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), nullptr);
    //3 coordinates of the vertex's normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    //the ID
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(6 * sizeof(GLfloat)));
    //whether it's selected or not, to simplify the code, a negative value means not selected while a positive value means selected
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(7 * sizeof(GLfloat)));
    m_vbo.release();
    m_vao.release();

    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, "../shaders/faces/vs.glsl");
    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, "../shaders/faces/fs.glsl");
    m_program.link();

    //get locations of uniforms
    m_program.bind();
    m_projMatrixLoc = m_program.uniformLocation("projMatrix");
    m_viewMatrixLoc = m_program.uniformLocation("mvMatrix");
    m_lightPosLoc = m_program.uniformLocation("lightPos");
    m_cameraPosLoc = m_program.uniformLocation("cameraPosition");
    m_colorLoc = m_program.uniformLocation("color");

    m_programPicking.addShaderFromSourceFile(QOpenGLShader::Vertex, "../shaders/faces/picking/vs.glsl");
    m_programPicking.addShaderFromSourceFile(QOpenGLShader::Fragment, "../shaders/faces/picking/fs.glsl");
    m_programPicking.link();

    //get locations of uniforms
    m_programPicking.bind();
    m_projMatrixPickingLoc = m_programPicking.uniformLocation("projMatrix");
    m_viewMatrixPickingLoc = m_programPicking.uniformLocation("mvMatrix");
}

void BatchFace::update() {
    m_vbo.bind();
    m_vbo.allocate(m_data.constData(), m_count * static_cast<int>(sizeof(GLfloat)));
    m_vbo.release();
}

void BatchFace::updateData() {
    m_count = 0;
    m_data.clear();

    if (m_mesh == nullptr) { return; }
    //we add data using triangles
    qsizetype nbTriangle = BatchFace::findNbOfTriangle(m_mesh);

    //for each triangle, there are 3 vertices
    qsizetype nbOfAdd = 3 * nbTriangle;

    //we resize the data for rapidity
    m_data.resize(nbOfAdd * 8);

    // set the ID to 1 for the mesh faces
    // it will be incremented for each face
    int ID = 1;

    //for each face
    for (he::Face* f: m_mesh->faces()) {
        this->addFace(f, ID);
        //going to the next face
        //we increment the ID
        ID++;
    }

    this->update();
}

void BatchFace::render(PickingType type) {
    switch (type) {
        case PickingType::PickingFace:
            m_programPicking.bind();
            m_vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, m_count / m_floatsPerVertex);
            m_programPicking.release();
            break;
        case PickingType::PickingEdge:
        case PickingType::PickingVertex:
        case PickingType::PickingCircle:
        case PickingType::PickingCircleDual:
            //draw only in depth buffer
            glColorMask(false, false, false, false);
            m_program.bind();
            m_vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, m_count / m_floatsPerVertex);
            m_program.release();
            glColorMask(true, true, true, true);
            break;
        case PickingType::PickingNone:
            bool cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
            if (!cullFaceEnabled) {
                glEnable(GL_CULL_FACE);
            }
            m_program.bind();
            m_vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, m_count / m_floatsPerVertex);
            m_program.release();
            if (!cullFaceEnabled) {
                glDisable(GL_CULL_FACE);
            }
            break;
    }
}

void BatchFace::setProjection(QMatrix4x4 projection) {
    m_program.bind();
    m_program.setUniformValue(m_projMatrixLoc, projection);
    m_program.release();

    m_programPicking.bind();
    m_programPicking.setUniformValue(m_projMatrixPickingLoc, projection);
    m_programPicking.release();
}

void BatchFace::setCamera(Camera camera) {
    m_program.bind();
    m_program.setUniformValue(m_viewMatrixLoc, camera.getViewMatrix());
    m_program.setUniformValue(m_cameraPosLoc, camera.getEye());
    m_program.release();

    m_programPicking.bind();
    m_programPicking.setUniformValue(m_viewMatrixPickingLoc, camera.getViewMatrix());
    m_programPicking.release();
}

void BatchFace::setLight(QVector3D lightPos) {
    m_program.bind();
    m_program.setUniformValue(m_lightPosLoc, lightPos);
    m_program.release();
}

void BatchFace::setColor(QColor const& color) {
    m_program.bind();
    m_program.setUniformValue(m_colorLoc, color);
    m_program.release();
}

void BatchFace::setMesh(he::Mesh* mesh) {
    m_mesh = mesh;
    m_selectedFace = 0;
    this->updateData();
}

void BatchFace::setSelectedFace(int faceIndex) {
    m_selectedFace = faceIndex;
}

he::Face* BatchFace::selectedFace() const {
    he::Face* res = nullptr;

    if (m_selectedFace - 1 >= 0 && m_selectedFace - 1 < static_cast<qsizetype>(m_mesh->faces().size())) {
        res = m_mesh->faces().at(m_selectedFace - 1);
    }

    return res;
}

qsizetype BatchFace::findNbOfTriangle(he::Mesh const* mesh) {
    qsizetype nb = 0;

    //for each face
    for (he::Face const* f: mesh->faces()) {
        //the number of triangle of a face
        //is the number of edges - 2
        nb += static_cast<qsizetype>(f->nbEdges() - 2);
    }

    return nb;
}

void BatchFace::addFace(he::Face* f, int ID) {
    // ear clipping method
    std::vector<he::Vertex*> vertices = f->allVertices();
    std::vector<std::size_t> markedVertices;

    // if the face is selected, we will
    // throw 1.0 and -1.0 otherwise
    float isSelected = (ID == m_selectedFace && m_selectedFace != 0) ? 1.0f : -1.0f;

    // find new base
    QMatrix4x4 pInv = f->basisChangeMatrix();

    std::size_t indexCurrent = 0;
    std::size_t countSinceLastMark = 0;
    while (markedVertices.size() <= vertices.size() - 3 && countSinceLastMark < vertices.size()) {
        // set indexAfter the next unmarked vertex after indexCurrent
        std::size_t indexNext = (indexCurrent + 1) % vertices.size();
        while (std::find(markedVertices.begin(), markedVertices.end(), indexNext) != markedVertices.end()) {
            indexNext = (indexNext + 1) % vertices.size();
        }

        // set indexPrev the previous unmarked vertex before indexCurrent
        std::size_t indexPrev = (indexCurrent + vertices.size() - 1) % vertices.size();
        while (std::find(markedVertices.begin(), markedVertices.end(), indexPrev) != markedVertices.end()) {
            indexPrev = (indexPrev + vertices.size() - 1) % vertices.size();
        }

        QVector3D pos0 = vertices[indexPrev]->pos();
        QVector3D pos1 = vertices[indexCurrent]->pos();
        QVector3D pos2 = vertices[indexNext]->pos();

        if (BatchFace::isValidTriangle(vertices[indexPrev], vertices[indexCurrent], vertices[indexNext], f, pInv)) {
            triangle(pos0, pos1, pos2, static_cast<float>(ID), isSelected);
            markedVertices.push_back(indexCurrent);
        } else {
            countSinceLastMark++;
        }

        // set indexCurrent the next unmarked vertex
        indexCurrent = (indexCurrent + 1) % vertices.size();
        while (std::find(markedVertices.begin(), markedVertices.end(), indexCurrent) != markedVertices.end()) {
            indexCurrent = (indexCurrent + 1) % vertices.size();
        }
    }
}

bool BatchFace::isValidTriangle(he::Vertex const* prev, he::Vertex const* current, he::Vertex const* next, he::Face const* face, QMatrix4x4 const& projMatrix) {
    std::vector<he::Vertex*> vertices = face->allVertices();
    std::vector<QVector2D> verticesPositions;
    for (he::Vertex const* v: vertices) {
        if (v != prev && v != current && v != next) {
            verticesPositions.push_back((projMatrix * QVector4D(v->pos(), 1.0f)).toVector2D());
        }
    }
    // project to plan of face to check the triangle validity
    QVector2D p0 = (projMatrix * QVector4D(prev->pos(), 1.0f)).toVector2D();
    QVector2D p1 = (projMatrix * QVector4D(current->pos(), 1.0f)).toVector2D();
    QVector2D p2 = (projMatrix * QVector4D(next->pos(), 1.0f)).toVector2D();

    // triangle must be convex
    // compute angle
    QVector2D v10 = p0 - p1;
    QVector2D v12 = p2 - p1;

    float angle = std::atan2(BatchFace::cross2D(v12, v10), QVector2D::dotProduct(v12, v10)) * 360.0f / (2.0f * M_PIf);
    if (angle <= 0.0f)
        return false;

    // check no points lies into the triangle
    for (QVector2D const& v: verticesPositions) {
        if (pointInTriangle(v, p0, p1, p2)) {
            return false;
        }
    }

    return true;
}

float BatchFace::cross2D(QVector2D const& u, QVector2D const& v) {
    return u.x() * v.y() - u.y() * v.x();
}

bool BatchFace::pointInTriangle(QVector2D const& P, QVector2D const& A, QVector2D const& B, QVector2D const& C) {
    QVector2D AB = B - A;
    QVector2D BC = C - B;
    QVector2D CA = A - C;

    QVector2D AP = P - A;
    QVector2D BP = P - B;
    QVector2D CP = P - C;

    float c1 = BatchFace::cross2D(AB, AP);
    float c2 = BatchFace::cross2D(BC, BP);
    float c3 = BatchFace::cross2D(CA, CP);

    bool hasNeg = (c1 < 0) || (c2 < 0) || (c3 < 0);
    bool hasPos = (c1 > 0) || (c2 > 0) || (c3 > 0);

    return !(hasNeg && hasPos);
}

void BatchFace::triangle(QVector3D const& pos1, QVector3D const& pos2, QVector3D const& pos3, float ID, float isSelected) {
    //compute the normal of the triangleSphere
    QVector3D n = QVector3D::normal(pos2 - pos1, pos3 - pos2);

    //add the vertices to the data
    this->addVertexFace(pos1, n, ID, isSelected);
    this->addVertexFace(pos2, n, ID, isSelected);
    this->addVertexFace(pos3, n, ID, isSelected);
}

void BatchFace::addVertexFace(QVector3D const& v, QVector3D const& n, float ID, float isSelected) {
    //add to the end of the data already added
    float* p = m_data.data() + m_count;
    //the coordinates of the vertex
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    //the normal of the vertex
    *p++ = n.x();
    *p++ = n.y();
    *p++ = n.z();
    //the ID of the face
    *p++ = ID;
    //whether the face is selected or not
    *p = isSelected;
    //we update the amount of data
    m_count += m_floatsPerVertex;
}

int BatchFace::renderOrder() {
    return 0;
}

int BatchFace::pickingOrder() {
    return 3;
}
