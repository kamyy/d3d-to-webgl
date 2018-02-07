#pragma once
#ifndef AnimActorObj_h
#define AnimActorObj_h

#include "RefFrame.h"
#include "Drawable.h"

namespace eve {
	using namespace std;

	class AnimJointObj : public RefFrame {
	public:
		AnimJointObj(RefFrame* parent);
	~	AnimJointObj();

	protected:
		AnimJointCls* m_parentCls;
		AnimJointCls* m_jointCls;
		AnimJointCls* m_childCls;
		AnimJointCls* m_nextCls;

		Matrix4x4 m_animMatrix;
	};

	class AnimActorObj : public RefFrame {
	public:
		AnimActor(RefFrame* parent);
	~	AnimActor();

	protected:
		vector<AnimJointObj*> m_jointObjs;
		AnimActorCls* m_actorCls;
	};
}