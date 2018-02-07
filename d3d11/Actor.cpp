#include "StdAfx.h"
#include "RefFrame.h"
#include "DrawableModel.h"
#include "Shader.h"
#include "Actor.h"

namespace d3d11demo {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    Actor::Actor(const char* name) : m_name(name) {

    }

    void Actor::addDrawableModel(shared_ptr<DrawableModel> model) {
        m_models.push_back(model);
    }

    void Actor::draw(bool bNoAlpha) const {
        for (auto p : m_models) {
            p->draw(bNoAlpha);
        }
    }

    void Actor::drawVertexNormals() const {
        for (auto p : m_models) {
            p->drawVertexNormals();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    ActorList::~ActorList() {
        for (auto p : *this) {
            delete p;
        }
    }

    void ActorList::draw(bool bNoAlpha) const {
        for (auto p : *this) {
            p->draw(bNoAlpha);
        }
    }

    void ActorList::drawVertexNormals() const {
        for (auto p : *this) {
            p->drawVertexNormals();
        }
    }

}
