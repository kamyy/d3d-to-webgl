#pragma once
#ifndef RefFrame_h
#define RefFrame_h

#include "Matrix4x4.h"
#include "Vector1x4.h"

namespace eve {
	class RefFrame : public RefCountedObject {
	public:
		RefFrame(RefFrame* parent = 0);
	~	RefFrame();

		void translate(const Vector1x4& v, const RefFrame* relative2 = 0);
		void rotateX(f32 theta, const RefFrame* relative2 = 0);
		void rotateY(f32 theta, const RefFrame* relative2 = 0);
		void rotateZ(f32 theta, const RefFrame* relative2 = 0);
		Vector1x4 map(const Vector1x4& v, RefFrame* tgt) const;

		Vector1x4 getPos(const RefFrame* tgt) const;
		const Matrix4x4& getLocalMatrix() const;
		const Matrix4x4& getWorldMatrix() const;

		void setLocalMatrix(const Matrix4x4& m);
		void setWorldMatrix(const Matrix4x4& m);

		RefFrame* getParent() const { 
			return m_parent; 
		}
		RefFrame* getChild() const { 
			return m_child; 
		}
		RefFrame* getNext() const { 
			return m_next; 
		}

		enum RefFrameType {
			REF_FRAME,
			CAMERA,
			OMNI_LIGHT,
			PRLL_LIGHT,
			ANIM_JOINT,
			ANIM_ACTOR
		};
		RefFrameType getType() { 
			return m_type; 
		}
		
	protected:
		RefFrameType m_type;
		mutable bool m_valid;
		RefFrame* m_parent;
		RefFrame* m_child;
		RefFrame* m_next;

		void invalidateSubtree();
		void validateAscending() const;
		virtual void invalidateSubClass() {}

		Matrix4x4 m_localMatrix;
		mutable Matrix4x4 m_worldMatrix;

	private:
		RefFrame(const RefFrame&);
		RefFrame operator=(const RefFrame&);
	};
}

#endif