#include "Widget.h"

#include <QtMath>
#include <QtGui/QOffscreenSurface>
#include <QtGui/QOpenGLContext>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setWindowTitle("OpenGL3, Qt6, C++");
    setFixedSize(QSize(400, 400));
}

Widget::~Widget()
{
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.86f, 0.87f, 0.84f, 1.f);

    float vertPositions[] = {
        -0.5f, -0.5f, 0.f,
        -0.5f, 0.5f, 0.f,
        0.5f, -0.5f, 0.f,
        0.5f, -0.5f, 0.f,
        -0.5f, 0.5f, 0.f,
        0.5f, 0.5f, 0.f
    };
    m_vertPosBuffer.create();
    m_vertPosBuffer.bind();
    m_vertPosBuffer.allocate(vertPositions, sizeof(vertPositions));

    m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Assets/Shaders/default.vert");
    m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Assets/Shaders/default.frag");
    m_program.bindAttributeLocation("aPosition", 0);
    m_program.link();

    m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3);
    m_program.enableAttributeArray(0);

    m_projMatrix.perspective(50.f, 1.f, 0.1f, 500.0f);
    m_viewMatrix.lookAt(QVector3D(0.f, 0.f, 20.f), QVector3D(0.f, 0.f, 0.f), QVector3D(0.f, 1.f, 0.f));

    m_modelMatrix.translate(0.f, 0.f);
    m_modelMatrix.rotate(30.f, QVector3D(0.f, 0.f, 1.f));
    m_modelMatrix.scale(7.f, 4.f);

    m_program.bind();

    m_uMvpMatrixLocation = m_program.uniformLocation("uMvpMatrix");

    int uColorLocation = m_program.uniformLocation("uColor");
    QVector3D color(0.34f, 0.73f, 0.73f);
    m_program.setUniformValue(uColorLocation, color);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    m_program.bind();

    QMatrix4x4 projViewMatrix = m_projMatrix * m_viewMatrix;
    m_mvpMatrix = projViewMatrix * m_modelMatrix;
    m_program.setUniformValue(m_uMvpMatrixLocation, m_mvpMatrix);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Widget::resizeGL(int w, int h)
{

}

void Widget::mousePressEvent(QMouseEvent *event)
{
    switch(event->button())
    {
        case Qt::LeftButton:
        {
            break;
        }
        case Qt::MiddleButton:
        {
            break;
        }
        case Qt::RightButton:
        {
            if (m_panning)
            {
                return;
            }
            m_panning = true;
            m_prevXForPan = event->position().x();
            m_prevYForPan = event->position().y();
            break;
        }
        default:
        {
            break;
        }
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_panning)
    {
        return;
    }

    float x = event->position().x();
    float y = event->position().y();

    float distance = m_viewMatrix.column(3).z();

    m_panX = (m_panX + (x - m_prevXForPan) * qFabs(distance) / 500.f);
    m_panY = (m_panY + (y - m_prevYForPan) * qFabs(distance) / 500.f);

    m_prevXForPan = x;
    m_prevYForPan = y;

    m_viewMatrix.setColumn(3, QVector4D(m_panX, -m_panY, distance, 1.f));

    update();
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if (!m_panning)
    {
        return;
    }
    m_panning = false;
}

void Widget::wheelEvent(QWheelEvent *event)
{
    float delta = event->angleDelta().y() / 100.f;
    float distance = m_viewMatrix.column(3).z();
    distance += delta;
    m_viewMatrix.setColumn(3, QVector4D(m_panX, -m_panY, distance, 1.f));
    update();
}
