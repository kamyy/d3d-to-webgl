class Drawable {
    constructor(gl, shader, vtxs, idxs) {
        this.shader = shader;
        
        this.vbuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, this.vbuffer);
        gl.bufferData(gl.ARRAY_BUFFER, this.vbuffer, gl.STATIC_DRAW);

        this.ibuffer = gl.createBuffer();
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.ibuffer);
        gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, this.ibuffer, gl.STATIC_DRAW);
    }
}
/*
#pragma once
#ifndef Drawable_h
#define Drawable_h

#include "D3D11Resource.h"
#include "D3D11Demo.h"
#include "RefFrame.h"
#include "Material.h"
#include "Shader.h"
#include "Math.h"

namespace d3d11demo {

    const float NORMAL_SCALE = 0.25f;
    const float TANGENT_SCALE = 0.25f;
    const float BINORMAL_SCALE = 0.25f;

    struct DrawBatchBase;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    struct AlphaBatch {
        float                m_depth;
        const DrawBatchBase* m_batch;

        AlphaBatch(float depth, const DrawBatchBase* batch)
            : m_depth(depth)
            , m_batch(batch) {

        }

        bool operator<(const AlphaBatch& rhs) const {
            return m_depth > rhs.m_depth; // furthest away takes precedence (draw back to front)
        };
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    struct DrawBatchBase : public D3D11Resource {
        static vector<AlphaBatch> s_alphaQ;
        const DrawableModel* m_model;
        const Material* m_mtl;
        Vector1x4 m_min;
        Vector1x4 m_max;

        static void flushAlphaQ();
        void addToAlphaQ() const;

        virtual void initD3D11Resources() override = 0;
        virtual void freeD3D11Resources() override = 0;
        virtual void drawPrimitives(bool noAlpha) const = 0;
        virtual void drawAllNormals() const = 0;

        explicit DrawBatchBase(const DrawableModel* model, const Material* mtl) : m_model(model), m_mtl(mtl) {}
        virtual ~DrawBatchBase() {};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    template<class T> struct DrawBatch : public DrawBatchBase {
        ID3D11Buffer* m_vtxBuf = nullptr;
        ID3D11Buffer* m_idxBuf = nullptr;
        ID3D11Buffer* m_nrmBuf = nullptr;

        vector<T>           m_vtxs;
        vector<uint16_t>    m_idxs;
        vector<Vertex_P3C4> m_nrms;

        virtual void initD3D11Resources() override {
            D3D11_BUFFER_DESC desc;
            D3D11_SUBRESOURCE_DATA data;
            ZeroMemory(&desc, sizeof(desc));
            ZeroMemory(&data, sizeof(data));

            desc.ByteWidth = UINT(sizeof(T) * m_vtxs.size());
            desc.Usage     = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            data.pSysMem   = &m_vtxs.front();
            ThrowIfFailed(g_app.getD3D11Device()->CreateBuffer(&desc, &data, &m_vtxBuf));

            desc.ByteWidth = UINT(sizeof(uint16_t) * m_idxs.size());
            desc.Usage     = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            data.pSysMem   = &m_idxs.front();
            ThrowIfFailed(g_app.getD3D11Device()->CreateBuffer(&desc, &data, &m_idxBuf));

            desc.ByteWidth = UINT(sizeof(Vertex_P3C4) * m_nrms.size());
            desc.Usage     = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            data.pSysMem   = &m_nrms.front();
            ThrowIfFailed(g_app.getD3D11Device()->CreateBuffer(&desc, &data, &m_nrmBuf));
        }

        virtual void freeD3D11Resources() override {
            if (m_vtxBuf) {
                m_vtxBuf->Release();
                m_vtxBuf = nullptr;
            }
            if (m_idxBuf) {
                m_idxBuf->Release();
                m_idxBuf = nullptr;
            }
            if (m_nrmBuf) {
                m_nrmBuf->Release();
                m_nrmBuf = nullptr;
            }
        }

        void initLocalResources(const vector<T>& vtxs, const vector<uint16_t>& idxs) {
            assert(!vtxs.empty());
            assert(!idxs.empty());

            m_vtxs = vtxs;
            m_idxs = idxs;

            m_min.m_x = m_max.m_x = vtxs[0].m_x;
            m_min.m_y = m_max.m_y = vtxs[0].m_y;
            m_min.m_z = m_max.m_z = vtxs[0].m_z;

            for (uint32_t i = 1; i < vtxs.size(); ++i) { // setup model space bounding box
                if (vtxs[i].m_x < m_min.m_x)
                    m_min.m_x = vtxs[i].m_x;
                if (vtxs[i].m_x > m_max.m_x)
                    m_max.m_x = vtxs[i].m_x;

                if (vtxs[i].m_y < m_min.m_y)
                    m_min.m_y = vtxs[i].m_y;
                if (vtxs[i].m_y > m_max.m_y)
                    m_max.m_y = vtxs[i].m_y;

                if (vtxs[i].m_z < m_min.m_z)
                    m_min.m_z = vtxs[i].m_z;
                if (vtxs[i].m_z > m_max.m_z)
                    m_max.m_z = vtxs[i].m_z;
            }

            for (auto& v : vtxs) { // setup vertex normals for rendering
                m_nrms.emplace_back(v.m_x,
                                    v.m_y,
                                    v.m_z,
                                    0.f, 0.f, 1.f, 1.f
                );
                m_nrms.emplace_back(v.m_x + v.m_nx * NORMAL_SCALE,
                                    v.m_y + v.m_ny * NORMAL_SCALE,
                                    v.m_z + v.m_nz * NORMAL_SCALE,
                                    0.f, 0.f, 1.f, 1.f
                );
            }
        }

        DrawBatch(const DrawableModel* model, const Material* mtl)
            : DrawBatchBase(model, mtl) {

        }

        DrawBatch(const DrawableModel* model, const Material* mtl, const vector<T>& vtxs, const vector<uint16_t>& idxs)
            : DrawBatchBase(model, mtl) {

            initLocalResources(vtxs, idxs);
            initD3D11Resources();
        }

        virtual ~DrawBatch() {
            freeD3D11Resources();
        }

        virtual void drawPrimitives(bool noAlpha) const override {
            if (noAlpha || !m_mtl->isTranslucent()) {

                UINT stride = sizeof(T);
                UINT offset = 0;

                m_mtl->applyMaterialToD3D11();
                g_app.getD3D11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                g_app.getD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_vtxBuf, &stride, &offset);
                g_app.getD3D11DeviceContext()->IASetIndexBuffer(m_idxBuf, DXGI_FORMAT_R16_UINT, 0);
                g_app.getD3D11DeviceContext()->DrawIndexed((UINT)m_idxs.size(), 0, 0);

            } else {

                addToAlphaQ();

            }
        }

        virtual void drawAllNormals() const override {
            UINT stride = sizeof(Vertex_P3C4);
            UINT offset = 0;

            Shader::get("P3C4")->applyShaderToD3D11();
            g_app.getD3D11DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
            g_app.getD3D11DeviceContext()->IASetVertexBuffers(0, 1, &m_nrmBuf, &stride, &offset);
            g_app.getD3D11DeviceContext()->Draw((UINT)m_nrms.size(), 0);
        }
    };

    struct DrawBatch_Vertex_P3N3B3T2 : public DrawBatch<Vertex_P3N3B3T2> {
        DrawBatch_Vertex_P3N3B3T2(const DrawableModel* model,
                                  const Material* mtl, const vector<Vertex_P3N3B3T2>& vtxs, const vector<uint16_t>& idxs)
            : DrawBatch<Vertex_P3N3B3T2>(model, mtl) {

            initLocalResources(vtxs, idxs);
            for (auto& v : vtxs) { // setup bump map normals for rendering
                m_nrms.emplace_back(v.m_x,
                                    v.m_y,
                                    v.m_z,
                                    0.f, 1.f, 0.f, 1.f
                );
                m_nrms.emplace_back(v.m_x + v.m_bx * BINORMAL_SCALE,
                                    v.m_y + v.m_by * BINORMAL_SCALE,
                                    v.m_z + v.m_by * BINORMAL_SCALE,
                                    0.f, 1.f, 0.f, 1.f
                );

                m_nrms.emplace_back(v.m_x,
                                    v.m_y,
                                    v.m_z,
                                    1.f, 0.f, 0.f, 1.f
                );
                m_nrms.emplace_back(v.m_x + (v.m_by * v.m_nz - v.m_bz * v.m_ny) * TANGENT_SCALE,
                                    v.m_y + (v.m_bz * v.m_nx - v.m_bx * v.m_nz) * TANGENT_SCALE,
                                    v.m_z + (v.m_bx * v.m_ny - v.m_by * v.m_nx) * TANGENT_SCALE,
                                    1.f, 0.f, 0.f, 1.f
                );
            }
            initD3D11Resources();
        }
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class DrawableModel : public RefFrame {
        vector<const DrawBatchBase*> m_drawBatches;

    public:
        DrawableModel(shared_ptr<RefFrame> parent);
        virtual ~DrawableModel();

        template<class T> void addDrawBatch(Material* mtl, const vector<T>& vtxs, const vector<uint16_t>& idxs) {
            m_drawBatches.push_back(new DrawBatch<T>(this, mtl, vtxs, idxs));
        }
        void addDrawBatch(Material* mtl, const vector<Vertex_P3N3B3T2>& vtxs, const vector<uint16_t>& idxs) {
            m_drawBatches.push_back(new DrawBatch_Vertex_P3N3B3T2(this, mtl, vtxs, idxs));
        }

        void applyModelSpaceToD3D11() const;
        void draw(bool bNoAlpha) const;
        void drawVertexNormals() const;
    };

}

*/
