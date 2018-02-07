#pragma once
#ifndef AnimSkeleton_h
#define AnimSkeleton_h

namespace eve {
	using namespace std;

	class AnimJointCls {
	public:
		bool read(istream& is);

		const char* getName() const { return m_name.c_str(); }

		u32 getParentID() const { return m_parentID; }
		u32 getJointID() const { return m_jointID; }
		u32 getChildID() const { return m_childID; }
		u32 getNextID() const { return m_nextID; }

		const Matrix4x4& getBindingMatrix() const { return m_bindingMatrix; }
		const Matrix4x4& getInverseMatrix() const { return m_inverseMatrix; }

	protected:
		string m_name;

		u32 m_parentID;
		u32 m_jointID;
		u32 m_childID;
		u32 m_nextID;

		Matrix4x4 m_bindingMatrix;
		Matrix4x4 m_inverseMatrix;
	};

	class AnimSkeleton : public RefCountedObject {
	public:
		AnimSkeleton();
	~	AnimSkeleton();
		bool read(istream& is);

		const char* getName() const { return m_name.c_str(); }
		const AnimJointCls* getJoint(u32 jointID) const;
		u32 getNumJoints() const;

	protected:
		string m_name;

		vector<AnimJointCls*> m_jointClasses;
	};
}

#endif