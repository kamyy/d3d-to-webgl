#pragma once
#ifndef Actor_h
#define Actor_h

namespace d3d11demo {
    class RefFrame;
    class DrawableModel;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class Actor {
        vector<shared_ptr<DrawableModel>> m_models; // index 0 holds root model
        string m_name;

    public:
        explicit Actor(const char* name);

        void addDrawableModel(shared_ptr<DrawableModel> model);

        void draw(bool bNoAlpha) const;
        void drawVertexNormals() const;
        const char* getName() const { return m_name.c_str(); }

        const DrawableModel* getModel(uint32_t i) const {
            return i < m_models.size() ? m_models.at(i).get() : nullptr;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class ActorList : public list<Actor*> {
    public:
       ~ActorList();
        void draw(bool bNoAlpha) const;
        void drawVertexNormals() const;
    };

}

#endif
