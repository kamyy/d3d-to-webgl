#pragma once
#ifndef AnimActorCls_h
#define AnimActorCls_h

#include "RefFrame.h"
#include "Drawable.h"

namespace eve {
	using namespace std;

	struct AnimJointReel {
		AnimJointReel();
	~	AnimJointReel();
		bool read(istream& is);

		vector<KeyFrame> m_keyFrames;

	};

	class AnimActorReel {
	public:
		AnimActorReel();
	~	AnimActorReel();
		bool read(istream& is);

		vector<AnimJointReel*> m_jointReels;
	};

	class AnimActorCls : public RefCountedObject {
	public:
		AnimActorCls();
	~	AnimActorCls();
		bool read(const char* filename);

		AnimActorObj* createObj();

	protected:
		string m_name;
		AnimSkeleton* m_rig;

		vector<AnimActorReel*> m_actorReels;


		AnimSkeleton* m_skeleton;
		Drawable* m_skinMesh;
	};
}

#endif
