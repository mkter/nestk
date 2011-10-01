/**
 * This file is part of the nestk library.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Nicolas Burrus <nicolas.burrus@uc3m.es>, (C) 2010
 */

#ifndef MESHVIEWER_H
#define MESHVIEWER_H

#include <ntk/core.h>
#include <ntk/mesh/mesh.h>

#include <QGLWidget>
#include <QPoint>

namespace ntk
{

class Pose3D;

class MeshViewer : public QGLWidget
{
    Q_OBJECT

public:
    enum MeshViewerMode { FLAT = 1, WIREFRAME = 2 };

public:
    MeshViewer(QWidget *parent = 0)
        : QGLWidget(parent),
          m_glcam_transform(4,4),
          m_mesh_center(0,0,0),
          m_mesh_origin(0,-1,-3),
          m_lookat_eye(0, 0, 1),
          m_lookat_center(0, 0, -5),
          m_lookat_up(0, 1, 0),
          m_use_vertex_buffer_object(false),
          m_show_grid(false)
    {
        cv::setIdentity(m_glcam_transform);
    }

    void addMesh(const ntk::Mesh& mesh, const Pose3D& pose, MeshViewerMode mode);
    void addMeshToDisplayList(const ntk::Mesh& mesh, const Pose3D& pose, MeshViewerMode mode);
    void addMeshToVertexBufferObject(const ntk::Mesh& mesh, const Pose3D& pose, MeshViewerMode mode);
    void addPlane(const ntk::Plane& plane);
    void swapScene();
    void setVertexBufferObjectMode(bool enable) { m_use_vertex_buffer_object = enable; }

    void resetCamera();
    void rotateCamera(const cv::Vec3f& axis,
                      double angle);
    void setCameraLookat(const cv::Vec3f& eye,
                         const cv::Vec3f& center,
                         const cv::Vec3f& up);
    void setDefaultCameraLookat(const cv::Vec3f& eye,
                                const cv::Vec3f& center,
                                const cv::Vec3f& up);
    void setShowGrid(bool show_it) { m_show_grid = show_it; }
    void setMeshOrigin(const cv::Point3f& p) { m_mesh_origin = p; }

    void enableLighting();

protected:
    virtual void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void  updateDisplayCenter();
    bool estimatePickingPoint(cv::Point3f& p, int mouse_x, int mouse_y);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent*);
    virtual void onCameraPositionUpdate(const cv::Vec3f& translation, const cv::Vec3f& rotation);
    void drawGrid();

private:
    struct VertexBufferObject {
        VertexBufferObject() : model_view_matrix(4,4) {}
        unsigned vertex_id;
        unsigned faces_id;
        int color_offset;
        int texture_offset;
        int nb_vertices;
        int nb_faces;
        bool has_texcoords;
        bool has_color;
        bool has_faces;
        cv::Mat_<GLfloat> model_view_matrix;
    };

protected:
    QPoint m_last_mouse_pos;
    std::vector<VertexBufferObject> m_vertex_buffer_objects;
    std::vector<VertexBufferObject> m_upcoming_vertex_buffer_objects;
    std::vector<int> m_display_lists;
    std::vector<int> m_upcoming_display_lists;
    std::vector<GLuint> m_textures;
    std::vector<GLuint> m_upcoming_textures;
    cv::Mat1f m_glcam_transform;
    cv::Point3f m_mesh_center;
    cv::Point3f m_mesh_origin;
    cv::Point3f m_display_center;
    cv::Vec3f m_lookat_eye;
    cv::Vec3f m_lookat_center;
    cv::Vec3f m_lookat_up;
    bool m_use_vertex_buffer_object;
    bool m_show_grid;
};

} // ntk

#endif // MESHVIEWER_H
