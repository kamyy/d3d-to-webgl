#include "StdAfx.h"
#include "RefFrame.h"

namespace d3d11demo {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    void RefFrame::invalidateSubtree() {
        if (m_validSubtree) {
            for (auto i = m_child; i != nullptr; i = i->m_next) {
                i->invalidateSubtree();
            }
            m_validSubtree = false;
            invalidateSubClass();
        }
    }

    void RefFrame::validateAscending() const {
        if (!m_validSubtree) {
            if (m_parent) {
                m_parent->validateAscending();
                m_modelMatrix = m_localMatrix * m_parent->m_modelMatrix;
            }
            m_validSubtree = true;
        }
    }

    RefFrame::RefFrame(shared_ptr<RefFrame> parent) {
        if (parent) {
            m_validSubtree  = false;
            m_parent        = parent;
            m_next          = parent->m_child;
            parent->m_child = this;
        }
    }

    RefFrame::~RefFrame() {
        if (m_parent) {
            for (auto i = m_parent->m_child; i->m_next != nullptr; i = i->m_next) {
                if (i->m_next == this) {
                    i->m_next = m_next;
                    break;
                }
            }
            if (m_parent->m_child == this) {
                m_parent->m_child = m_next;
            }
        }
    }

    const Matrix4x4& RefFrame::getLocalMatrix() const {
        return m_localMatrix;
    }

    const Matrix4x4& RefFrame::getModelMatrix() const {
        validateAscending();
        return m_modelMatrix;
    }

    void RefFrame::setLocalMatrix(const Matrix4x4& mtx) {
        m_localMatrix = mtx;
        invalidateSubtree();
    }

    void RefFrame::setModelMatrix(const Matrix4x4& mtx) {
        // worldMatrix = localMatrix * parent->worldMatrix
        // worldMatrix / parent->worldMatrix = localMatrix
        m_localMatrix = mtx * !m_parent->getModelMatrix();
        invalidateSubtree();
    }

    void RefFrame::translate(const Vector1x4& v, const RefFrame* relative2) {
        if (relative2 == this || relative2 == nullptr) { // relative to own axes
            const float x = m_localMatrix.m_41;
            const float y = m_localMatrix.m_42;
            const float z = m_localMatrix.m_43;
            m_localMatrix.m_41 = x + (v.m_x * m_localMatrix.m_11) + (v.m_y * m_localMatrix.m_21) + (v.m_z * m_localMatrix.m_31);
            m_localMatrix.m_42 = y + (v.m_x * m_localMatrix.m_12) + (v.m_y * m_localMatrix.m_22) + (v.m_z * m_localMatrix.m_32);
            m_localMatrix.m_43 = z + (v.m_x * m_localMatrix.m_13) + (v.m_y * m_localMatrix.m_23) + (v.m_z * m_localMatrix.m_33);

        } else if (relative2->m_parent == nullptr) { // relative to root axes
            Vector1x4 d = relative2->mapPos(Vector1x4(v.m_x, v.m_y, v.m_z, 0.f), m_parent.get());
            m_localMatrix.m_41 += d.m_x;
            m_localMatrix.m_42 += d.m_y;
            m_localMatrix.m_43 += d.m_z;

        } else if (relative2 == m_parent.get()) { // relative to parent axes
            m_localMatrix.m_41 += v.m_x;
            m_localMatrix.m_42 += v.m_y;
            m_localMatrix.m_43 += v.m_z;

        } else { // relative to arbitrary axes
            assert(m_parent.get());
            Vector1x4 d = relative2->mapPos(Vector1x4(v.m_x, v.m_y, v.m_z, 0.f), m_parent.get());
            m_localMatrix.m_41 += d.m_x;
            m_localMatrix.m_42 += d.m_y;
            m_localMatrix.m_43 += d.m_z;
        }
        invalidateSubtree();
    }

    void RefFrame::rotateX(float theta, const RefFrame* relative2) {
        if (relative2 == this || relative2 == nullptr) { // relative to own axes
            m_localMatrix = Matrix4x4(Matrix4x4::ROTATE_X, theta) * m_localMatrix;

        } else if (relative2 == m_parent.get()) { // relative to parent
            m_localMatrix = m_localMatrix * Matrix4x4(Matrix4x4::ROTATE_X, theta);

        } else { // relative to arbitrary axes
            assert(false);
        }
        invalidateSubtree();
    }

    void RefFrame::rotateY(float theta, const RefFrame* relative2) {
        if (relative2 == this || relative2 == nullptr) { // relative to own axes
            m_localMatrix = Matrix4x4(Matrix4x4::ROTATE_Y, theta) * m_localMatrix;

        } else if (relative2 == m_parent.get()) { // relative to parent
            m_localMatrix = m_localMatrix * Matrix4x4(Matrix4x4::ROTATE_Y, theta);

        } else { // relative to arbitrary axes
            assert(false);
        }
        invalidateSubtree();
    }

    void RefFrame::rotateZ(float theta, const RefFrame* relative2) {
        if (relative2 == this || relative2 == nullptr) { // relative to own axes
            m_localMatrix = Matrix4x4(Matrix4x4::ROTATE_Z, theta) * m_localMatrix;

        } else if (relative2 == m_parent.get()) { // relative to parent
            m_localMatrix = m_localMatrix * Matrix4x4(Matrix4x4::ROTATE_Z, theta);

        } else { // relative to arbitrary axes
            assert(false);
        }
        invalidateSubtree();
    }

    Vector1x4 RefFrame::mapPos(const Vector1x4& v, const RefFrame* tgt) const {
        if (tgt == m_parent.get()) {
            return v * m_localMatrix;
        } else if (tgt->m_parent == nullptr) {
            return v * getModelMatrix();
        } else {
            return v * getModelMatrix() * !tgt->getModelMatrix();
        }
    }

    json RefFrame::toJSON() const {
        const Matrix4x4& m = getModelMatrix();

        json node = {
            { "nodeType", "RefFrame" },
            { "modelMatrix", { m.m_11, m.m_12, m.m_13, m.m_14, m.m_21, m.m_22, m.m_23, m.m_24, m.m_31, m.m_32, m.m_33, m.m_34, m.m_41, m.m_42, m.m_43, m.m_44 } },
        };

        for (auto p = getChild(); p != nullptr; p = p->getNext()) {
            node["children"].push_back(p->toJSON());
        }

        return node;
    }

}
