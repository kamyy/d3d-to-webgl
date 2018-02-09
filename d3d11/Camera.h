#pragma once
#ifndef Camera_h
#define Camera_h

#include "RefFrame.h"

#include "json/single_include/nlohmann/json.hpp"

namespace d3d11demo {

    using json = nlohmann::json;

    const float CAMERA_DEFAULT_N_CLIP_DISTANCE = 0.50f;
    const float CAMERA_DEFAULT_F_CLIP_DISTANCE = 100.f;
    const float CAMERA_DEFAULT_FOV = 1.0f;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class Camera : public RefFrame {
    public:
        Camera( float fieldOfView,
                float aspectRatio,
                float clipDistanceN,
                float clipDistanceF,
                shared_ptr<RefFrame> parent);

        virtual ~Camera() {

        }

        float getFieldOfView() const {
            return m_fieldOfView;
        }
        float getAspectRatio() const {
            return m_aspectRatio;
        }

        float getClipDistanceN() const {
            return m_clipDistanceN;
        }
        float getClipDistanceF() const {
            return m_clipDistanceF;
        }

        void setFieldOfView(float fieldOfView) {
            m_fieldOfView = fieldOfView;
        }
        void setAspectRatio(float aspectRatio) {
            m_aspectRatio = aspectRatio;
        }

        void setClipDistanceN(float clipDistance) {
            m_clipDistanceN = clipDistance;
        }
        void setClipDistanceF(float clipDistance) {
            m_clipDistanceF = clipDistance;
        }

        const Matrix4x4 getViewMatrix();
        const Matrix4x4 getProjMatrix();
        const Matrix4x4 getViewProjMatrix();

        virtual json toJSON() const override;

    protected:
        float m_fieldOfView;
        float m_aspectRatio;
        float m_clipDistanceN;
        float m_clipDistanceF;

        Matrix4x4 m_view;
        Matrix4x4 m_proj;

    private:
        Camera(const Camera&);
        Camera operator=(const Camera&) const;
    };

    typedef std::vector<shared_ptr<Camera>> CameraArray;

}

#endif



