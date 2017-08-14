#include "StdAfx.h"
#include "Shader.h"
#include "DrawableModel.h"

namespace d3d11demo {
    vector<AlphaBatch> DrawBatchBase::s_alphaQ;

    void DrawBatchBase::flushAlphaQ() {
        if (s_alphaQ.size()) {
            sort(s_alphaQ.begin(), s_alphaQ.end());

            g_app.alphaBlendingOn();
            for (auto& b : s_alphaQ) {
                b.m_batch->m_model->applyModelSpaceToD3D11();
                b.m_batch->drawPrimitives(true);
            }
            g_app.noAlphaBlending();

            s_alphaQ.clear();
        }
    }

    void DrawBatchBase::addToAlphaQ() const {
        // material is translucent so delay rendering it until the end of the frame
        // by adding it to a queue to be later sorted by y in view space
        Matrix4x4  viewMatrix = g_app.getActiveCamera()->getViewMatrix();
        AlphaBatch alphaBatch(0.0f, this);
        s_alphaQ.push_back(alphaBatch);

        // min x min y min z corner of bbox in model space
        Vector1x4 p(m_min.m_x,
                    m_min.m_y,
                    m_min.m_z, 1.f);

        alphaBatch.m_depth = (p.m_x * viewMatrix.m_12) + (p.m_y * viewMatrix.m_22) + (p.m_z * viewMatrix.m_32) + viewMatrix.m_42;

        // max x min y min z corner of bbox in model space
        p.m_x = m_max.m_x;
        p.m_y = m_min.m_y;
        p.m_z = m_min.m_z;
        p.m_w = (p.m_x * viewMatrix.m_12) + (p.m_y * viewMatrix.m_22) + (p.m_z * viewMatrix.m_32) + viewMatrix.m_42;
        alphaBatch.m_depth = (alphaBatch.m_depth > p.m_w) ? alphaBatch.m_depth : p.m_w;

        // max x min y max z corner of bbox in model space
        p.m_x = m_max.m_x;
        p.m_y = m_min.m_y;
        p.m_z = m_max.m_z;
        p.m_w = (p.m_x * viewMatrix.m_12) + (p.m_y * viewMatrix.m_22) + (p.m_z * viewMatrix.m_32) + viewMatrix.m_42;
        alphaBatch.m_depth = (alphaBatch.m_depth > p.m_w) ? alphaBatch.m_depth : p.m_w;

        // min x min y max z corner of bbox in model space
        p.m_x = m_min.m_x;
        p.m_y = m_min.m_y;
        p.m_z = m_max.m_z;
        p.m_w = (p.m_x * viewMatrix.m_12) + (p.m_y * viewMatrix.m_22) + (p.m_z * viewMatrix.m_32) + viewMatrix.m_42;
        alphaBatch.m_depth = (alphaBatch.m_depth > p.m_w) ? alphaBatch.m_depth : p.m_w;

        // min x max y min z corner of bbox in model space
        p.m_x = m_min.m_x;
        p.m_y = m_max.m_y;
        p.m_z = m_min.m_z;
        p.m_w = (p.m_x * viewMatrix.m_12) + (p.m_y * viewMatrix.m_22) + (p.m_z * viewMatrix.m_32) + viewMatrix.m_42;

        // max x max y min z corner of bbox in model space
        p.m_x = m_max.m_x;
        p.m_y = m_max.m_y;
        p.m_z = m_min.m_z;
        p.m_w = (p.m_x * viewMatrix.m_12) + (p.m_y * viewMatrix.m_22) + (p.m_z * viewMatrix.m_32) + viewMatrix.m_42;
        alphaBatch.m_depth = (alphaBatch.m_depth > p.m_w) ? alphaBatch.m_depth : p.m_w;

        // max x max y max z corner of bbox in model space
        p.m_x = m_max.m_x;
        p.m_y = m_max.m_y;
        p.m_z = m_max.m_z;
        p.m_w = (p.m_x * viewMatrix.m_12) + (p.m_y * viewMatrix.m_22) + (p.m_z * viewMatrix.m_32) + viewMatrix.m_42;
        alphaBatch.m_depth = (alphaBatch.m_depth > p.m_w) ? alphaBatch.m_depth : p.m_w;

        // min x max y max z corner of bbox in model space
        p.m_x = m_min.m_x;
        p.m_y = m_max.m_y;
        p.m_z = m_max.m_z;
        p.m_w = (p.m_x * viewMatrix.m_12) + (p.m_y * viewMatrix.m_22) + (p.m_z * viewMatrix.m_32) + viewMatrix.m_42;
        alphaBatch.m_depth = (alphaBatch.m_depth > p.m_w) ? alphaBatch.m_depth : p.m_w;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    void DrawableModel::applyModelSpaceToD3D11() const {
        static Vector1x4 origin(0.0, 0.0, 0.0, 1.0f);
        static Vector1x4 up(0.0, 0.0, 1.0, 0.0f);
        VS_REG_B1 vs;
        PS_REG_B1 ps;

        vs.g_model_view_proj_matrix = getModelMatrix() * g_app.getActiveCamera()->getViewProjMatrix(); // model view proj matrix
        vs.g_camera_pos = g_app.getActiveCamera()->mapPos(origin, this); // camera position in model space
        vs.g_omniLS_pos = g_app.getOmniDirLS()->mapPos(origin, this); // omniDirLS position in model space
        vs.g_up_dir = ps.g_up_dir = up * !getModelMatrix(); // world up in model space
        Shader::setVS_REG_B1(vs);
        Shader::setPS_REG_B1(ps);
    }

    DrawableModel::DrawableModel(shared_ptr<RefFrame> parent) : RefFrame(parent) {

    }

    DrawableModel::~DrawableModel() {
        for (auto p : m_drawBatches) {
            delete p;
        }
    }

    void DrawableModel::draw(bool bNoAlpha) const {
        applyModelSpaceToD3D11();
        for (auto p : m_drawBatches) {
            p->drawPrimitives(bNoAlpha);
        }
    }

    void DrawableModel::drawVertexNormals() const {
        applyModelSpaceToD3D11();
        for (auto p : m_drawBatches) {
            p->drawAllNormals();
        }
    }

}
