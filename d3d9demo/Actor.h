#pragma once
#ifndef Actor_h
#define Actor_h

#include "Vertex.h"
#include "RefFrame.h"
#include "Material.h"
#include "Drawable.h"
#include "D3DResourceUser.h"

namespace eve {
	using namespace std;

	class Actor : public RefCountedObject {
	public:
		void add(RefFrame* frm, Drawable* drw);
		RefFrame* getRefFrame(u32 i) {
			return m_frms.at(i);
		}
		void drawNormals() const;
		void drawAtOnce() const;
		void draw() const;
		const char* getName() const { return m_name.c_str(); }
		Actor(const char* name);
	~	Actor();
	protected:
		vector<RefFrame*> m_frms; // index 0 holds root frm
		vector<Drawable*> m_drws; // index 0 holds root drw
		string m_name;
	};

	class ActorList : public list<Actor*> {
	public:
		ActorList() {}
	~	ActorList();
		void draw() const;
		void drawAtOnce() const;
		void drawNormals() const;
	private:
		ActorList(const ActorList&);
		ActorList operator=(const ActorList&);
	};
}

#endif
