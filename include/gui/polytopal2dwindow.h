#ifndef AUTOFRAC_POLYTOPAL2DWINDOW_H
#define AUTOFRAC_POLYTOPAL2DWINDOW_H

#include <QTimer>
#include <QColorDialog>
#include "halfedge/mesh.h"
#include "batchsphere.h"
#include "batchskybox.h"
#include "batchdebugline.h"
#include "batchface.h"
#include "batchedge.h"
#include "batchvertex.h"
#include "batchcircle.h"
#include "batchcircledual.h"
#include "polytopal/inversivecoordinates.h"
#include "batchplane.h"

class QStatusBar;

class QProgressBar;

class GLView;

QT_BEGIN_NAMESPACE
namespace Ui { class Polytopal2DWindow; }
QT_END_NAMESPACE

class Polytopal2DWindow : public QWidget {
Q_OBJECT

public:
    explicit Polytopal2DWindow(QWidget* parent = nullptr);
    ~Polytopal2DWindow() override;

    void updateCircles();
    void updateCirclesDual();
    void openOBJFile(QString const& file);
    void setInfo(std::string const& textInfo, int timeoutMs = 2000);

    void updateDataFaces();
    he::Face* selectedFace();
    void setSelectedFace(int faceIndex);

    void updateDataEdges();
    he::HalfEdge* selectedEdge();
    void setSelectedEdge(int edgeIndex);

    void updateDataVertices();
    he::Vertex* selectedVertex();
    he::Vertex* selectedVertex2();
    void setSelectedVertex(int vertexIndex);
    void setSelectedVertex2(int vertexIndex);

    void updateDataCircles();
    void setSelectedCircle(int circleIndex);
    gui::Circle* selectedCircle();

    void setSelectedCircleDual(int circleIndex);
    gui::Circle* selectedCircleDual();

    void updateDataMesh();
    void updateData();

    he::Mesh* mesh();

    void removeSelectedCircle();
    void removeSelectedCircleDual();

public slots:
    [[maybe_unused]] void slotOpenOBJFile();
    [[maybe_unused]] void slotExportAllFaces();
    [[maybe_unused]] void slotExportSelectedFace();
    [[maybe_unused]] void slotDisplayUnitSphereChanged();
    [[maybe_unused]] void slotCanonizeMesh();
    void canonicalizeStep();
    [[maybe_unused]] void slotDisplayAreaCircles();
    [[maybe_unused]] void slotDisplayDualAreaCircles();
    [[maybe_unused]] void slotDisplayMeshClicked();
    [[maybe_unused]] void slotIncreaseInversion();
    [[maybe_unused]] void slotDecreaseInversion();
    [[maybe_unused]] void slotProjectCirclesClicked();
    void increaseInversion();
    [[maybe_unused]] void slotExportOBJ();

    [[maybe_unused]] void slotUpdateLabelPrecision(int value);

    [[maybe_unused]] void slotBarycentricSubdivision();
    [[maybe_unused]] void slotGeneralizedBarycentricSubdivision();
    [[maybe_unused]] void slotLoopSubdivision();

    [[maybe_unused]] void slotChangeTheme();

    [[maybe_unused]] void slotChangeUserData();
    [[maybe_unused]] void slotChangeAllUserData();

    [[maybe_unused]] void slotScaleUpCircles();
    [[maybe_unused]] void slotScaleDownCircles();

    [[maybe_unused]] void slotOBJFromCircles();
    [[maybe_unused]] void slotOBJOfCircles();

    [[maybe_unused]] void slotUpdateSkyBox(int index);
    [[maybe_unused]] void slotChangeMeshColor();
    [[maybe_unused]] void slotChangeMeshTransparency(int value);
    [[maybe_unused]] void slotScaleMesh();
    [[maybe_unused]] void slotCullFaceChanged();
    [[maybe_unused]] void slotSphereRenderTypeChanged(int newValue);
    [[maybe_unused]] void slotDisplayProjectionPoint();
    [[maybe_unused]] void slotDisplayPlane();
    [[maybe_unused]] void slotDisplayPolar();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void updateEnablementPoly();
    void setInfoAdvancement(int percent);
    void updateBatchCircles(bool dual);

private:
    //animations projection
    std::vector<gui::Circle> m_circlesBeginProjection;
    std::vector<gui::Circle> m_circlesDualBeginProjection;

    std::vector<gui::Circle> m_circlesEndProjection;
    std::vector<gui::Circle> m_circlesDualEndProjection;

    float m_tProjection = 0.0;

    QTimer m_timerProjection;

    void initAnimationProjection();
    void endAnimationProjection();

public slots:
    void animatingProjection();

private:
    //animations inversion
    std::vector<poly::InversiveCoordinates> m_circlesBeginInversion;
    std::vector<poly::InversiveCoordinates> m_circlesEndInversion;

    float m_tInversion = 0.0;

    QTimer m_timerInversion;

    void initAnimationInversion();
    void endAnimationInversion();

public slots:
    void animatingInversion();

private:
    Ui::Polytopal2DWindow* ui;

    QStatusBar* m_statusBar;

    BatchSphere m_batchSphere;
    BatchSkyBox m_batchSkybox;
    BatchDebugLine m_batchDebugLine;
    BatchFace m_batchFace;
    BatchEdge m_batchEdge;
    BatchVertex m_batchVertex;
    BatchCircle m_batchCircle;
    BatchCircleDual m_batchCircleDual;
    BatchPlane m_batchPlane;

    he::Mesh m_mesh;
    GLView* m_view = nullptr;
    bool m_openedMesh;

    QTimer m_timerCanonicalize;
    std::vector<poly::InversiveCoordinates> m_circles;
    std::vector<poly::InversiveCoordinates> m_circlesDual;
    int m_inversionLevel;
    std::size_t m_nbInversions = 0;
    std::size_t m_circlesIndex;

    double m_firstError = -1.0;

    QProgressBar* m_progressBar = nullptr;

    bool m_canonicalized = false;
    int m_step = 0;

    QVector3D m_colorWhiteTheme { 0.0f, 0.0f, 0.0f };
    QVector3D m_colorDarkTheme { 1.0f, 1.0f, 1.0f };
    QVector3D m_colorWhiteThemeDual { 1.0f, 0.0f, 1.0f };
    QVector3D m_colorDarkThemeDual { 1.0f, 0.0f, 0.0f };
    QColorDialog m_colorDialog;
};


#endif //AUTOFRAC_POLYTOPAL2DWINDOW_H
