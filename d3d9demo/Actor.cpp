#include "BasicTypes.h"
#include "Shader.h"
#include "Actor.h"

namespace eve {
	//
	// Actor
	//
	void Actor::add(RefFrame* frm, Drawable* drw) {
		m_frms.push_back(frm);
		m_drws.push_back(drw);
		frm->addRef();
		drw->addRef();
	}
	void Actor::draw() const {
		for (u32 i = 0; i < m_frms.size(); ++i) {
			Shader::setRefFrame(m_frms[i]);
			m_drws[i]->draw();
		}
	}
	void Actor::drawAtOnce() const {
		for (u32 i = 0; i < m_frms.size(); ++i) {
			Shader::setRefFrame(m_frms[i]);
			m_drws[i]->drawAtOnce();
		}
	}
	void Actor::drawNormals() const {
		for (u32 i = 0; i < m_frms.size(); ++i) {
			Shader::setRefFrame(m_frms[i]);
			m_drws[i]->drawNormals();
		}
	}
	Actor::Actor(const char* name) : m_name(name) {

	}
	Actor::~Actor() {
		for (u32 i = 0; i < m_frms.size(); ++i) {
			m_frms[i]->rmvRef();
			m_drws[i]->rmvRef();
		}
	}

	//
	// Actor List
	//
	ActorList::~ActorList() {
		for (iterator i = begin(); i != end(); ++i) {
			(*i)->rmvRef();
		}
	}
	void ActorList::draw() const {
		for (const_iterator i = begin(); i != end(); ++i) {
			(*i)->draw();
		}
	}
	void ActorList::drawAtOnce() const {
		for (const_iterator i = begin(); i != end(); ++i) {
			(*i)->drawAtOnce();
		}
	}
	void ActorList::drawNormals() const {
		for (const_iterator i = begin(); i != end(); ++i) {
			(*i)->drawNormals();
		}
	}
}