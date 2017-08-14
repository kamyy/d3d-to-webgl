#include "StdAfx.h"
#include "Camera.h"

namespace d3d11demo {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    Camera::Camera( float fieldOfView,
                    float aspectRatio,
                    float clipDistanceN,
                    float clipDistanceF,
                    shared_ptr<RefFrame> parent) : RefFrame(parent) {

        m_fieldOfView = fieldOfView;
        m_aspectRatio = aspectRatio;
        m_clipDistanceN = clipDistanceN;
        m_clipDistanceF = clipDistanceF;
    }

    const Matrix4x4 Camera::getViewMatrix() {
        // right-handed camera system where +ve y-axis points in view direction
        m_view = !getModelMatrix();
        return m_view;
    }

    const Matrix4x4 Camera::getProjMatrix() {
        // standard D3D left-handed projection matrix
        DirectX::XMMATRIX xm = DirectX::XMMatrixPerspectiveFovLH(m_fieldOfView,
                                                                 m_aspectRatio,
                                                                 m_clipDistanceN,
                                                                 m_clipDistanceF);
        memcpy(&m_proj, &xm, sizeof(m_proj));

        // flip y and z vertex coordinates to convert from RHS to LHS
        // for D3D by switching around rows 2 and 3
        const float x = m_proj.m_21;
        const float y = m_proj.m_22;
        const float z = m_proj.m_23;
        const float w = m_proj.m_24;
        m_proj.m_21 = m_proj.m_31;
        m_proj.m_22 = m_proj.m_32;
        m_proj.m_23 = m_proj.m_33;
        m_proj.m_24 = m_proj.m_34;
        m_proj.m_31 = x;
        m_proj.m_32 = y;
        m_proj.m_33 = z;
        m_proj.m_34 = w;

        return m_proj;
    }

    const Matrix4x4 Camera::getViewProjMatrix() {
        return getViewMatrix() * getProjMatrix();
    }

}
