#include "BasicTypes.h"
#include "RefFrame.h"

namespace eve {
	RefFrame::RefFrame(RefFrame* p) : m_type(REF_FRAME) {
		m_localMatrix.setIdentity();
		m_worldMatrix.setIdentity();
		if (p) {
			m_valid = false;
			p->addRef();
			m_parent= p;
			m_child	= 0;
			m_next	= p->m_child;
			p->m_child = this;
		} else {
			m_valid = true;
			m_parent= 0;
			m_child	= 0;
			m_next	= 0;
		}
	}

	RefFrame::~RefFrame() {
		if (m_parent) {
			RefFrame* i;
			for (i = m_parent->m_child; i->m_next != 0; i = i->m_next) {
				if (i->m_next == this) {
					i->m_next = m_next;
					break;
				}
			}
			if (m_parent->m_child == this) {
				m_parent->m_child = m_next;
			}
			m_parent->rmvRef();
		}
	}

	void RefFrame::translate(const Vector1x4& v, const RefFrame* relative2) {
		if (relative2 == this || relative2 == 0) { // relative to own axes
			const f32 x = m_localMatrix.m_41;
			const f32 y = m_localMatrix.m_42;
			const f32 z = m_localMatrix.m_43;
			m_localMatrix.m_41 = x + (v.m_x * m_localMatrix.m_11) + (v.m_y * m_localMatrix.m_21) + (v.m_z * m_localMatrix.m_31);
			m_localMatrix.m_42 = y + (v.m_x * m_localMatrix.m_12) + (v.m_y * m_localMatrix.m_22) + (v.m_z * m_localMatrix.m_32);
			m_localMatrix.m_43 = z + (v.m_x * m_localMatrix.m_13) + (v.m_y * m_localMatrix.m_23) + (v.m_z * m_localMatrix.m_33);

		} else if (relative2->m_parent == 0) { // relative to root axes
			Vector1x4 d = relative2->map(Vector1x4(v.m_x, v.m_y, v.m_z, 0.f), m_parent);
			m_localMatrix.m_41 += d.m_x;
			m_localMatrix.m_42 += d.m_y;
			m_localMatrix.m_43 += d.m_z;

		} else if (relative2 == m_parent) { // relative to parent axes
			m_localMatrix.m_41 += v.m_x;
			m_localMatrix.m_42 += v.m_y;
			m_localMatrix.m_43 += v.m_z;

		} else { // relative to arbitrary axes
			EVE_ASSERT(this->m_parent);
			Vector1x4 d = relative2->map(Vector1x4(v.m_x, v.m_y, v.m_z, 0.f), m_parent);
			m_localMatrix.m_41 += d.m_x;
			m_localMatrix.m_42 += d.m_y;
			m_localMatrix.m_43 += d.m_z;
		}
		invalidateSubtree();
	}

	void RefFrame::rotateX(f32 theta, const RefFrame* relative2) {
		if (relative2 == this || relative2 == 0) { // relative to own axes
			m_localMatrix = Matrix4x4(Matrix4x4::ROTATE_X, theta) * m_localMatrix;

		} else if (relative2 == m_parent) { // relative to parent
			m_localMatrix = m_localMatrix * Matrix4x4(Matrix4x4::ROTATE_X, theta);

		} else { // relative to arbitrary axes
			EVE_ASSERT(false);
		}
		invalidateSubtree();
	}
	void RefFrame::rotateY(f32 theta, const RefFrame* relative2) {	
		if (relative2 == this || relative2 == 0) { // relative to own axes
			m_localMatrix = Matrix4x4(Matrix4x4::ROTATE_Y, theta) * m_localMatrix;

		} else if (relative2 == m_parent) { // relative to parent
			m_localMatrix = m_localMatrix * Matrix4x4(Matrix4x4::ROTATE_Y, theta);

		} else {
			EVE_ASSERT(false);
		}
		invalidateSubtree();
	}
	void RefFrame::rotateZ(f32 theta, const RefFrame* relative2) {
		if (relative2 == this || relative2 == 0) { // relative to own axes
			m_localMatrix = Matrix4x4(Matrix4x4::ROTATE_Z, theta) * m_localMatrix;

		} else if (relative2 == m_parent) { // relative to parent
			m_localMatrix = m_localMatrix * Matrix4x4(Matrix4x4::ROTATE_Z, theta);

		} else {
			EVE_ASSERT(false);
		}
		invalidateSubtree();
	}
	Vector1x4 RefFrame::map(const Vector1x4& v, RefFrame* tgt) const {
		if (tgt == m_parent) { 
			return v * m_localMatrix;
		} else if (tgt->m_parent == 0) { 
			return v * getWorldMatrix();
		} else {
			return v * getWorldMatrix() * !tgt->getWorldMatrix();
		}
	}

	Vector1x4 RefFrame::getPos(const RefFrame* tgt) const {
		static const Vector1x4 origin(0.0f, 0.0f, 0.0f, 1.0f);
		if (tgt == m_parent) {
			return Vector1x4(	m_localMatrix.m_41, 
								m_localMatrix.m_42, 
								m_localMatrix.m_43, 1.f);
		} else if (tgt->m_parent == 0) {
			return origin * getWorldMatrix();
		} else {
			return origin * getWorldMatrix() * !tgt->getWorldMatrix();
		}
	}
	const Matrix4x4& RefFrame::getLocalMatrix() const {
		return m_localMatrix;
	}
	const Matrix4x4& RefFrame::getWorldMatrix() const {
		validateAscending();
		return m_worldMatrix;
	}

	void RefFrame::setLocalMatrix(const Matrix4x4& mtx) {
		m_localMatrix = mtx;
		invalidateSubtree();
	}
	void RefFrame::setWorldMatrix(const Matrix4x4& mtx) {
		// worldMatrix = localMatrix * parent->worldMatrix
		// worldMatrix / parent->worldMatrix = localMatrix
		m_localMatrix = mtx * !m_parent->getWorldMatrix();
		invalidateSubtree();
	}

	void RefFrame::invalidateSubtree() {
		if (m_valid) {
			for (RefFrame* i = m_child; i != 0; i = i->m_next) {
				i->invalidateSubtree();
			}
			m_valid = false;
			invalidateSubClass();
		}
	}	
	void RefFrame::validateAscending() const {
		if (!m_valid) {
			if (m_parent) {
				m_parent->validateAscending();
				m_worldMatrix = m_localMatrix * m_parent->m_worldMatrix;
			}
			m_valid = true;
		}
	}
}