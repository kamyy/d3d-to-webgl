#pragma once
#ifndef ASE_h
#define ASE_h

#include "Camera.h"
#include "Actor.h"

namespace eve {
	using namespace std;

	class ASE {
	public:
		void createCameras(CameraArray& cameras, CamTgtArray& camTgts, RefFrame* parent) const;
		void createActors(ActorList* actors, RefFrame* parent) const;
		bool read(const char* filename);
		ASE();
	~	ASE();
	private:
		static bool skipBlock(istream& is);
		struct Scn {
			u32 m_begFrame;
			u32 m_endFrame;
			u32 m_frameSpeed;
			u32 m_frameTicks;
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
			~	DiffMap();
				Texture* m_texture;
			};
			struct NormMap {
				bool read(istream& is);
				NormMap();
			~	NormMap();
				Texture* m_texture;
			};
			vector<Mtl*> m_mtls;
			Material* m_mtl;
			f32 m_uOffset;
			f32 m_vOffset;
			f32 m_uTiling;
			f32 m_vTiling;
			Mtl();
		~	Mtl();
			bool read(istream& is, const string& parent = "");
		};
		struct Geo {
			struct Vtx {
				f32 m_x;
				f32 m_y;
				f32 m_z;
			};
			struct Tuv {
				f32 m_u;
				f32 m_v;
				f32 m_w;
			};
			struct Tri {
				Vtx* m_vtx1;
				Vtx* m_vtx2;
				Vtx* m_vtx3;
				Tuv* m_tuv1;
				Tuv* m_tuv2;
				Tuv* m_tuv3;
				u32 m_mtlID;
				Vector1x4 m_nrm1;
				Vector1x4 m_nrm2;
				Vector1x4 m_nrm3;
				Vector1x4 m_nrmF;
			};
			u32 m_mtlID;
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
		~	Geo();

			void calcBinormal(Vtx_P3N3B3T2& vtx1, const Vtx_P3N3B3T2& vtx2, const Vtx_P3N3B3T2& vtx3) const;
			void addTrisTypeA(Drawable* drw, const ASE::Mtl* mtl, u32 mtlID) const;
			void addTrisTypeB(Drawable* drw, const ASE::Mtl* mtl, u32 mtlID) const;
			void addTrisTypeC(Drawable* drw, const ASE::Mtl* mtl, u32 mtlID) const;
			void addMulMtlTris(Drawable* drw, const ASE::Mtl* mtl) const;
			void addStdMtlTris(Drawable* drw, const ASE::Mtl* mtl, u32 mtlID = UINT_MAX) const;
		};
		struct Cam {
			f32 m_fov;
			f32 m_clipN;
			f32 m_clipF;
			Matrix4x4 m_camMtx;
			Matrix4x4 m_tgtMtx;
			bool read(istream& is);
			bool readset(istream& is);
			bool readMtx(istream& is, Matrix4x4& mtx);
			Cam();
		};
		void addDrawable(Actor* actor, RefFrame* parent, const Geo* geo) const;
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
