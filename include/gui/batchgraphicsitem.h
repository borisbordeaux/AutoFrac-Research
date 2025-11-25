#ifndef AUTOFRAC_BATCHGRAPHICSITEM_H
#define AUTOFRAC_BATCHGRAPHICSITEM_H

#include <QOpenGLExtraFunctions>
#include <utility>
#include "gui/camera.h"
#include "gui/pickingtype.h"

class BatchGraphicsItem : protected QOpenGLExtraFunctions {

public:
    BatchGraphicsItem() = default;

    /**
     * default destructor
     */
    virtual ~BatchGraphicsItem()  = default;

    /**
     * initialize the object (must call initializeOpenGLFunctions())
     */
    virtual void init() {}

    /**
     * send data to GPU
     */
    virtual void update() {}

    /**
     * update data to send to GPU
     */
    virtual void updateData() {}

    /**
     * render depending on the given picking type
     */
    virtual void render(PickingType type) = 0;

    /**
     * set a projection matrix
     */
    virtual void setProjection(QMatrix4x4 /*projection*/) {}

    /**
     * set a camera (matrix, position...)
     */
    virtual void setCamera(Camera /*camera*/) {}

    /**
     * set light position
     */
    virtual void setLight(QVector3D /*lightPos*/) {}

    /**
     * set inverse of viewport
     */
    virtual void setInvViewport(float /*x*/, float /*y*/) {}

    /**
     * set color
     */
    virtual void setColor(QColor const& /*color*/) {}

    /**
     * an object with a high renderOrder will be drawn before the others
     * @return the renderOrder of the object
     */
    virtual int renderOrder() = 0;

    virtual int pickingOrder() = 0;
};


#endif //AUTOFRAC_BATCHGRAPHICSITEM_H
