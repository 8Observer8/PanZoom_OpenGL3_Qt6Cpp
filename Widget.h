#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QOpenGLFunctions>
#include <QtGui/QMatrix4x4>
#include <QtGui/QWheelEvent>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtOpenGL/QOpenGLBuffer>
#include <QtOpenGL/QOpenGLShaderProgram>

class Widget : public QOpenGLWidget, QOpenGLFunctions
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent (QMouseEvent *event) override;
    void mouseMoveEvent (QMouseEvent *event) override;
    void mouseReleaseEvent (QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QOpenGLShaderProgram m_program;
    QOpenGLBuffer m_vertPosBuffer;
    QMatrix4x4 m_projMatrix;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_modelMatrix;
    QMatrix4x4 m_mvpMatrix;
    int m_uMvpMatrixLocation;

    bool m_panning = false;
    float m_panX = 0;
    float m_panY = 0;
    float m_prevXForPan;
    float m_prevYForPan;
};
#endif // WIDGET_H
