#pragma once
#ifndef ASE_h
#define ASE_h

#include "Camera.h"
#include "Color.h"
#include "Math.h"

namespace d3d11demo {

    class Actor;
    class Texture;
    class RefFrame;
    class Material;
    class DrawableModel;
    struct Vertex_P3N3B3T2;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class ASE {
    public:
        void createCameras(CameraArray& cameras, shared_ptr<RefFrame> parent) const;
        void createActors(class ActorList* actors, shared_ptr<RefFrame> parent) const;
        bool read(const char* filename);

        ASE();
       ~ASE();

    private:
        static bool skipBlock(istream& is);

        struct Scn {
            uint32_t m_begFrame;
            uint32_t m_endFrame;
            uint32_t m_frameSpeed;
            uint32_t m_frameTicks;
            Color m_ambient;
            Color m_bground;
            Scn();
            bool read(istream& is);
        };

        struct Mtl {
            enum Class {
                CLASS_STANDARD = 0,
                CLASS_MULTISUB = 1,
                CLASS_BITMAP = 2
            };
            struct DiffMap {
                bool read(istream& is, Mtl* mtl);
                DiffMap();
               ~DiffMap();
                Texture* m_texture;
            };
            struct NormMap {
                bool read(istream& is);
                NormMap();
               ~NormMap();
                Texture* m_texture;
            };
            vector<Mtl*> m_mtls;
            Material* m_mtl;
            float m_uOffset;
            float m_vOffset;
            float m_uTiling;
            float m_vTiling;
            Mtl();
        ~   Mtl();
            bool read(istream& is, const string& parent = "");
        };

        struct Geo {
            struct Vtx {
                float m_x;
                float m_y;
                float m_z;
            };

            struct Tuv {
                float m_u;
                float m_v;
                float m_w;
            };

            struct Tri {
                Vtx* m_vtx1;
                Vtx* m_vtx2;
                Vtx* m_vtx3;
                Tuv* m_tuv1;
                Tuv* m_tuv2;
                Tuv* m_tuv3;
                uint32_t m_mtlID;
                Vector1x4 m_nrm1;
                Vector1x4 m_nrm2;
                Vector1x4 m_nrm3;
                Vector1x4 m_nrmF;
            };

            uint32_t m_mtlID;
            string m_myName;
            string m_parent;
            Matrix4x4 m_mtx;
            vector<Vtx> m_vtxs;
            vector<Tri> m_tris;
            vector<Tuv> m_tuvs;
            bool read(istream& is);
            bool readNode(istream& is);
            bool readMesh(istream& is);
            bool readAnim(istream& is);
            bool readMeshVtx(istream& is);
            bool readMeshTri(istream& is);
            bool readMeshNrm(istream& is);
            bool readMeshVtxTUV(istream& is);
            bool readMeshTriTUV(istream& is);
            Geo();
        ~   Geo();

            void calcBinormal(Vertex_P3N3B3T2& vtx1, const Vertex_P3N3B3T2& vtx2, const Vertex_P3N3B3T2& vtx3) const;
            void addTrisTypeA(DrawableModel* model, const ASE::Mtl* mtl, uint32_t mtlID) const;
            void addTrisTypeB(DrawableModel* model, const ASE::Mtl* mtl, uint32_t mtlID) const;
            void addTrisTypeC(DrawableModel* model, const ASE::Mtl* mtl, uint32_t mtlID) const;
            void addMulMtlTris(DrawableModel* model, const ASE::Mtl* mtl) const;
            void addStdMtlTris(DrawableModel* model, const ASE::Mtl* mtl, uint32_t mtlID = UINT_MAX) const;
        };

        struct Cam {
            float m_fov;
            float m_clipN;
            float m_clipF;
            Matrix4x4 m_camMtx;
            Matrix4x4 m_tgtMtx;
            bool read(istream& is);
            bool readset(istream& is);
            bool readMtx(istream& is, Matrix4x4& mtx);
            Cam();
        };

        void addDrawableModel(Actor* actor, shared_ptr<RefFrame> parent, const Geo* geo) const;
        bool read(istream& is);
        typedef vector<Mtl*> MtlArray;
        typedef vector<Geo*> GeoArray;
        typedef vector<Cam*> CamArray;
        MtlArray m_mtls;
        GeoArray m_geos;
        CamArray m_cams;
        Scn m_scn;
    };

}

#endif
