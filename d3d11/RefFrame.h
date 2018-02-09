#pragma once
#ifndef RefFrame_h
#define RefFrame_h

#include "Math.h"

#include "json/single_include/nlohmann/json.hpp"

namespace d3d11demo {

    using json = nlohmann::json;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class RefFrame {
        mutable bool m_validSubtree = true;;

        shared_ptr<RefFrame> m_parent = nullptr;
        RefFrame* m_child  = nullptr;
        RefFrame* m_next   = nullptr;

        Matrix4x4         m_localMatrix = Matrix4x4(Matrix4x4::IDENTITY);
        mutable Matrix4x4 m_modelMatrix = Matrix4x4(Matrix4x4::IDENTITY);

        void invalidateSubtree();
        void validateAscending() const;
        virtual void invalidateSubClass() {}

        RefFrame(const RefFrame&) = delete;
        RefFrame operator=(const RefFrame&) = delete;

    public:
        RefFrame(shared_ptr<RefFrame> parent = nullptr);
        virtual ~RefFrame();

        shared_ptr<RefFrame> getParent() { return m_parent; }

        RefFrame* getChild() const { return m_child; }
        RefFrame* getChild() { return m_child; }

        RefFrame* getNext() const { return m_next; }
        RefFrame* getNext() { return m_next; }

        const Matrix4x4& getLocalMatrix() const;
        const Matrix4x4& getModelMatrix() const;
        void setLocalMatrix(const Matrix4x4& m);
        void setModelMatrix(const Matrix4x4& m);

        void translate(const Vector1x4& v, const RefFrame* relative2 = nullptr);
        void rotateX(float theta, const RefFrame* relative2 = nullptr);
        void rotateY(float theta, const RefFrame* relative2 = nullptr);
        void rotateZ(float theta, const RefFrame* relative2 = nullptr);

        Vector1x4 mapPos(const Vector1x4& v, const RefFrame* tgt) const;

        virtual json toJSON() const;
    };
}

#endif
