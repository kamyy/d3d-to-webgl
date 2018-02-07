#include "StdAfx.h"

#include "Actor.h"
#include "Shader.h"
#include "Texture.h"
#include "RefFrame.h"
#include "Material.h"
#include "DrawableModel.h"

#include "ASE.h"

namespace d3d11demo {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    ASE::ASE() {
    }

    ASE::~ASE() {
        for (GeoArray::iterator i = m_geos.begin(); i < m_geos.end(); ++i)
            delete (*i);
        for (MtlArray::iterator j = m_mtls.begin(); j < m_mtls.end(); ++j)
            delete (*j);
        for (CamArray::iterator k = m_cams.begin(); k < m_cams.end(); ++k)
            delete (*k);
    }

    bool ASE::skipBlock(istream& is) {
        char buf[512];
        uint32_t block = 1;
        is.getline(buf, sizeof(buf));
        while (!is.fail()) {
            if (strchr(buf, '}')) {
                if (--block == 0) {
                    break;
                }
            } else if (strchr(buf, '{')) {
                ++block;
            }
            is.getline(buf, sizeof(buf));
        }
        return !is.fail();
    }

    void ASE::createCameras(CameraArray& cameras, shared_ptr<RefFrame> parent) const {
        assert(cameras.empty());

        for (CamArray::const_iterator i = m_cams.begin(); i < m_cams.end(); ++i) {
            auto tgt = make_shared<RefFrame>(parent);
            auto cam = make_shared<Camera>(CAMERA_DEFAULT_FOV, float(RENDERING_WINDOW_WD) / (RENDERING_WINDOW_HT),
                                           (*i)->m_clipN,
                                           (*i)->m_clipF, tgt);
            tgt->setModelMatrix((*i)->m_tgtMtx);
            cam->setModelMatrix((*i)->m_camMtx);
            cameras.push_back(cam);
        }
    }

    void ASE::createActors(ActorList* actors, shared_ptr<RefFrame> parent) const {
        assert(actors->empty());

        for (uint32_t i = 0; i < m_geos.size(); ++i) {
            const string& myNameStr = m_geos[i]->m_myName;
            const string& parentStr = m_geos[i]->m_parent;
            if (parentStr.empty()) { // must be a root object
                Actor* actor = new Actor(myNameStr.c_str());
                addDrawableModel(actor, parent, m_geos[i]);
                actors->push_back(actor);
            } else { // look for the parent node
                uint32_t j;
                for (j = 0; j < m_geos.size(); ++j) {
                    if (j != i && m_geos[j]->m_myName == parentStr) {
                        break;
                    }
                }
                if (j == m_geos.size()) { // no parent node found
                    Actor* actor = new Actor(myNameStr.c_str());
                    addDrawableModel(actor, parent, m_geos[i]);
                    actors->push_back(actor);
                }
            }
        }
    }

    bool ASE::read(const char* filename) {
        char *p, *q, buf[512];
        ifstream is(filename);

        is.getline(buf, sizeof(buf));
        if (is.fail() || !strstr(buf, "*3DSMAX_ASCIIEXPORT" ))
            return false;
        is.getline(buf, sizeof(buf));
        if (is.fail() || !strstr(buf, "*COMMENT"            ))
            return false;
        is.getline(buf, sizeof(buf));
        if (is.fail() || !strstr(buf, "*SCENE"              ))
            return false;

        if (!m_scn.read(is)) {
            return false;
        }

        is.getline(buf, sizeof(buf));
        if (is.fail() || !strstr(buf, "*MATERIAL_LIST"      ))
            return false;

        is.getline(buf, sizeof(buf));
        while (!is.fail()) {
            p = strtok(buf, " \t\n");
            q = strlen(p) + p + 1;
            if (!strcmp(p, "*MATERIAL")) {
                Mtl* mtl = new Mtl;
                if (!mtl->read(is)) {
                    delete mtl;
                    return false;
                }
                m_mtls.push_back(mtl);
            } else if (strchr(p, '}')) {
                break;
            } else if (strchr(q, '{')) {
                if (!skipBlock(is))
                    return false;
            }
            is.getline(buf, sizeof(buf));
        }

        return read(is);
    }

    bool ASE::read(istream& is) {
        char *p, *q, buf[512];

        is.getline(buf, sizeof(buf));
        while (!is.fail()) {
            p = strtok(buf, " \t\n");
            q = strlen(p) + p + 1;

            if (!strcmp(p, "*GROUP")) {
                if (!read(is)) {
                    return false;
                }
            } else if (!strcmp(p,   "*GEOMOBJECT"   )) {
                Geo* geo = new Geo;
                if (!geo->read(is)) {
                    delete geo;
                    return false;
                }
                m_geos.push_back(geo);
            } else if (!strcmp(p,   "*CAMERAOBJECT" )) {
                Cam* cam = new Cam;
                if (!cam->read(is)) {
                    delete cam;
                    return false;
                }
                m_cams.push_back(cam);
            } else if (strchr(p, '}')) {
                break;
            } else if (strchr(q, '{')) {
                if (!skipBlock(is))
                    return false;
            }
            is.getline(buf, sizeof(buf));
        }
        return true;
    }

    void ASE::addDrawableModel(Actor* actor, shared_ptr<RefFrame> parent, const Geo* geo) const {
        auto model = make_shared<DrawableModel>(parent);

        uint32_t i;
        for (i = 0; i < m_mtls.size(); ++i) {
            if (geo->m_mtlID == i) {
                if (m_mtls[i]->m_mtls.empty()) {
                    geo->addStdMtlTris(model.get(), m_mtls[i]); // triangles reference one material
                } else {
                    geo->addMulMtlTris(model.get(), m_mtls[i]); // triangles grouped by material
                }
                break;
            }
        }
        if (i == m_mtls.size()) {
            geo->addTrisTypeA(model.get(), 0, UINT_MAX);
        }
        model->setModelMatrix(geo->m_mtx);
        actor->addDrawableModel(model);

        for (uint32_t i = 0; i < m_geos.size(); ++i) {
            // search for and add any child geometry
            const string& s = m_geos[i]->m_parent;
            if (!s.empty() && s == geo->m_myName) {
                addDrawableModel(actor, model, m_geos[i]);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    ASE::Scn::Scn() {
        m_begFrame = 0;
        m_endFrame = 0;
        m_frameSpeed = 30;
        m_frameTicks = 120;
    }

    bool ASE::Scn::read(istream& is) {
        char *p, *q, buf[512];

        is.getline(buf, sizeof(buf));
        while (!is.fail()) {
            p = strtok(buf, " \t\n");
            q = strlen(p) + p + 1;
            if (!strcmp(p, "*SCENE_LASTFRAME")) {
                if (sscanf(q, "%d", &m_endFrame) != 1)
                    return false;
            } else if (!strcmp(p, "*SCENE_FIRSTFRAME")) {
                if (sscanf(q, "%d", &m_begFrame) != 1)
                    return false;
            } else if (!strcmp(p, "*SCENE_FRAMESPEED")) {
                if (sscanf(q, "%d", &m_frameSpeed) != 1)
                    return false;
            } else if (!strcmp(p, "*SCENE_TICKSPERFRAME")) {
                if (sscanf(q, "%d", &m_frameTicks) != 1)
                    return false;
            } else if (!strcmp(p, "*SCENE_AMBIENT_STATIC")) {
                if (sscanf(q, "%f %f %f", &m_ambient.m_r, &m_ambient.m_g, &m_ambient.m_b) != 3)
                    return false;
            } else if (!strcmp(p, "*SCENE_BACKGROUND_STATIC")) {
                if (sscanf(q, "%f %f %f", &m_bground.m_r, &m_bground.m_g, &m_bground.m_b) != 3)
                    return false;
            } else if (strchr(p, '}')) {
                break;
            } else if (strchr(q, '{')) {
                if (!skipBlock(is))
                    return false;
            }
            is.getline(buf, sizeof(buf));
        }
        return !is.fail();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    ASE::Mtl::Mtl() {
        m_mtl = 0;
        m_uOffset = 0.f;
        m_vOffset = 0.f;
        m_uTiling = 1.f;
        m_vTiling = 1.f;
    }

    ASE::Mtl::~Mtl() {
        ASE::MtlArray::iterator i;
        for (i = m_mtls.begin(); i < m_mtls.end(); ++i) {
            delete (*i);
        }
    }

    bool ASE::Mtl::read(istream& is, const string& parentName) {
        char *p, *q, buf[512];

        Class clss = CLASS_STANDARD;
        Color ambi;
        Color diff;
        Color spec;
        float shineExponent = 0.0f;
        float shineStrength = 0.0f;
        DiffMap diffMap;
        NormMap bumpMap;
        string mtlName;

        is.getline(buf, sizeof(buf));
        while (!is.fail()) {
            p = strtok(buf, " \t\n");
            q = strlen(p) + p + 1;

            if (!strcmp(p, "*MATERIAL_NAME")) {
                mtlName = strtok(strchr(q, '"'), "\"");
                mtlName = mtlName + parentName;
            } else if (!strcmp(p, "*MATERIAL_CLASS")) {
                if (strstr(q, "Multi/Sub-Object")) {
                    clss = CLASS_MULTISUB;
                } else if (strstr(q, "Standard" )) {
                    clss = CLASS_STANDARD;
                } else {
                    assert(!"Bad mtl class");
                }
            } else if (!strcmp(p, "*MATERIAL_AMBIENT" )) {
                if (sscanf(q, "%f %f %f", &ambi.m_r, &ambi.m_g, &ambi.m_b) != 3)
                    return false;
            } else if (!strcmp(p, "*MATERIAL_DIFFUSE" )) {
                if (sscanf(q, "%f %f %f", &diff.m_r, &diff.m_g, &diff.m_b) != 3)
                    return false;
            } else if (!strcmp(p, "*MATERIAL_SPECULAR")) {
                if (sscanf(q, "%f %f %f", &spec.m_r, &spec.m_g, &spec.m_b) != 3)
                    return false;
            } else if (!strcmp(p, "*MATERIAL_SHINESTRENGTH" )) {
                if (sscanf(q, "%f", &shineStrength) != 1)
                    return false;
            } else if (!strcmp(p, "*MATERIAL_SHINE"         )) {
                if (sscanf(q, "%f", &shineExponent) != 1)
                    return false;
            } else if (!strcmp(p, "*SUBMATERIAL")) {
                Mtl* mtl = new Mtl;
                if (!mtl->read(is, mtlName)) {
                    delete mtl;
                    return false;
                }
                m_mtls.push_back(mtl);
            } else if (!strcmp(p, "*MAP_DIFFUSE")) {
                if (!diffMap.read(is, this)) {
                    return false;
                }
            } else if (!strcmp(p, "*MAP_BUMP")) {
                if (!bumpMap.read(is)) {
                    return false;
                }
            } else if (strchr(p, '}')) {
                break;
            } else if (strchr(q, '{')) {
                if (!skipBlock(is))
                    return false;
            }
            is.getline(buf, sizeof(buf));
        }

        bool ok = !is.fail();
        if (ok) {
            m_mtl = Material::get(mtlName.c_str());
            m_mtl->setDiff(Color(diff.m_r, diff.m_g, diff.m_b));
            m_mtl->setSpec(Color(   spec.m_r * shineStrength,
                                    spec.m_g * shineStrength,
                                    spec.m_b * shineStrength));
            m_mtl->setShininess(max(1.0f, shineExponent * 100.0f));

            Shader* shaderA = Shader::get("P3N3B3T2");
            Shader* shaderB = Shader::get("P3N3T2");
            Shader* shaderC = Shader::get("P3N3");
            m_mtl->setShader(shaderC);

            if (diffMap.m_texture) {
                m_mtl->setTexture(0, diffMap.m_texture);
                m_mtl->setShader(shaderB);
            }
            if (bumpMap.m_texture) {
                m_mtl->setTexture(1, bumpMap.m_texture);
                m_mtl->setShader(shaderA);
            }
        }
        return ok;
    }

    //
    // ASE Material Diffuse Map
    //
    ASE::Mtl::DiffMap::DiffMap() {
        m_texture = 0;
    }

    ASE::Mtl::DiffMap::~DiffMap() {
    }

    bool ASE::Mtl::DiffMap::read(istream& is, Mtl* mtl) {
        char *p, *q, buf[512];
        string file;

        is.getline(buf, sizeof(buf));
        while (!is.fail()) {
            p = strtok(buf, " \t\n");
            q = strlen(p) + p + 1;

            if (!strcmp(p, "*BITMAP")) {
                file = strtok(strchr(q, '"'), "\"");
            } else if (!strcmp(p, "*MAP_CLASS")) {
                assert(strstr(q, "Bitmap"));
            } else if (!strcmp(p, "*UVW_U_OFFSET")) {
                if (sscanf(q, "%f", &mtl->m_uOffset) != 1)
                    return false;
            } else if (!strcmp(p, "*UVW_V_OFFSET")) {
                if (sscanf(q, "%f", &mtl->m_vOffset) != 1)
                    return false;
            } else if (!strcmp(p, "*UVW_U_TILING")) {
                if (sscanf(q, "%f", &mtl->m_uTiling) != 1)
                    return false;
            } else if (!strcmp(p, "*UVW_V_TILING")) {
                if (sscanf(q, "%f", &mtl->m_vTiling) != 1)
                    return false;
            } else if (strchr(p, '}')) {
                break;
            } else if (strchr(q, '{')) {
                if (!skipBlock(is))
                    return false;
            }
            is.getline(buf, sizeof(buf));
        }
        bool ok = !is.fail();
        if (ok) {
            if ((m_texture = Texture::get(file)) != nullptr) {
                m_texture->setSamplerStateDesc(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP);
            }
        }
        return ok;
    }

    //
    // ASE Material Bump Map
    //
    ASE::Mtl::NormMap::NormMap() {
        m_texture = 0;
    }

    ASE::Mtl::NormMap::~NormMap() {
    }

    bool ASE::Mtl::NormMap::read(istream& is) {
        char *p, *q, buf[512];
        string file;

        is.getline(buf, sizeof(buf));
        while (!is.fail()) {
            p = strtok(buf, " \t\n");
            q = strlen(p) + p + 1;

            if (!strcmp(p, "*BITMAP")) {
                file = strtok(strchr(q, '"'), "\"");
            } else if (!strcmp(p, "*MAP_CLASS")) {
                assert(strstr(q, "Bitmap"));
            } else if (strchr(p, '}')) {
                break;
            } else if (strchr(q, '{')) {
                if (!skipBlock(is))
                    return false;
            }
            is.getline(buf, sizeof(buf));
        }
        bool ok = !is.fail();
        if (ok) {
            if ((m_texture = Texture::get(file)) != nullptr) {
                m_texture->setSamplerStateDesc(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP);
            }
        }
        return ok;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    bool ASE::Geo::read(istream& is) {
        char *p, *q, buf[512];

        is.getline(buf, sizeof(buf));
        while (!is.fail()) {
            p = strtok(buf, " \t\n");
            q = strlen(p) + p + 1;
            if (!strcmp(p, "*NODE_NAME")) {
                m_myName = strtok(strchr(q, '"'), "\"");
            } else if (!strcmp(p, "*NODE_PARENT" )) {
                m_parent = strtok(strchr(q, '"'), "\"");
            } else if (!strcmp(p, "*MATERIAL_REF")) {
                if (sscanf(q, "%d", &m_mtlID) < 1)
                    return false;
            } else if (!strcmp(p, "*NODE_TM")) {
                if (!readNode(is))
                    return false;
            } else if (!strcmp(p, "*MESH"   )) {
                if (!readMesh(is))
                    return false;
            } else if (strchr(p, '}')) {
                break;
            } else if (strchr(q, '{')) {
                if (!skipBlock(is))
                    return false;
            }
            is.getline(buf, sizeof(buf));
        }
        return !is.fail();
    }

    bool ASE::Geo::readNode(istream& is) {
        char *p, *q, buf[512];

        is.getline(buf, sizeof(buf));
        while (!is.fail()) {
            p = strtok(buf, " \t\n");
            q = strlen(p) + p + 1;
            if (!strcmp(p, "*TM_ROW0")) {
                if (sscanf(q, "%f %f %f", &m_mtx.m_11, &m_mtx.m_12, &m_mtx.m_13) != 3)
                    return false;
            } else if (!strcmp(p, "*TM_ROW1")) {
                if (sscanf(q, "%f %f %f", &m_mtx.m_21, &m_mtx.m_22, &m_mtx.m_23) != 3)
                    return false;
            } else if (!strcmp(p, "*TM_ROW2")) {
                if (sscanf(q, "%f %f %f", &m_mtx.m_31, &m_mtx.m_32, &m_mtx.m_33) != 3)
                    return false;
            } else if (!strcmp(p, "*TM_ROW3")) {
                if (sscanf(q, "%f %f %f", &m_mtx.m_41, &m_mtx.m_42, &m_mtx.m_43) != 3)
                    return false;
            } else if (strchr(p, '}')) {
                break;
            } else if (strchr(q, '{')) {
                if (!skipBlock(is))
                    return false;
            }
            is.getline(buf, sizeof(buf));
        }
        bool ok = !is.fail();
        if (ok) { // remove any scaling in the matrix
            ((Vector1x4*)&m_mtx.m_11)->normalize();
            ((Vector1x4*)&m_mtx.m_21)->normalize();
            ((Vector1x4*)&m_mtx.m_31)->normalize();
        }
        return ok;
    }

    bool ASE::Geo::readMesh(istream& is) {
        char *p, *q, buf[512];

        is.getline(buf, sizeof(buf));
        while (!is.fail()) {
            p = strtok(buf, " \t\n");
            q = strlen(p) + p + 1;
            if (!strcmp(p, "*MESH_VERTEX_LIST")) {
                if (!readMeshVtx(is)) // are in world space
                    return false;
            } else if (!strcmp(p, "*MESH_FACE_LIST")) {
                if (!readMeshTri(is))
                    return false;
            } else if (!strcmp(p, "*MESH_TVERTLIST")) {
                if (!readMeshVtxTUV(is))
                    return false;
            } else if (!strcmp(p, "*MESH_TFACELIST")) {
                if (!readMeshTriTUV(is))
                    return false;
            } else if (!strcmp(p, "*MESH_NORMALS")) {
                if (!readMeshNrm(is)) // are in model space
                    return false;
            } else if (strchr(p, '}')) {
                break;
            } else if (strchr(q, '{')) {
                if (!skipBlock(is))
                    return false;
            }
            is.getline(buf, sizeof(buf));
        }
        bool ok = !is.fail();
        if (ok) { // transform vertex positions from world space into local space
            Matrix4x4 inv = !m_mtx;
            for (uint32_t i = 0; i < m_vtxs.size(); ++i) {
                Vector1x4 pos(  m_vtxs[i].m_x,
                                m_vtxs[i].m_y,
                                m_vtxs[i].m_z, 1.f);
                pos *= inv;
                m_vtxs[i].m_x = pos.m_x;
                m_vtxs[i].m_y = pos.m_y;
                m_vtxs[i].m_z = pos.m_z;
            }
        }
        return ok;
    }

    bool ASE::Geo::readMeshVtx(istream& is) {
        Vtx v; char buf[512];

        is.getline(buf, sizeof(buf));
        while (!is.fail() && !strchr(buf, '}')) {
            if (sscanf(buf, "%*s %*d %f %f %f", &v.m_x, &v.m_y, &v.m_z) != 3) {
                return false;
            }
            is.getline(buf, sizeof(buf));
            m_vtxs.push_back(v);
        }
        return !is.fail();
    }

    bool ASE::Geo::readMeshTri(istream& is) {
        char* p, buf[512];
        uint32_t idx1;
        uint32_t idx2;
        uint32_t idx3;
        Tri t;

        is.getline(buf, sizeof(buf));
        while (!is.fail() && !strchr(buf, '}')) {
            if (sscanf( buf, "%*s %*d %*s %*s%d %*s%d %*s%d", &idx1, &idx2, &idx3) != 3) {
                return false;
            }
            p = strstr(buf, "*MESH_MTLID");
            if (sscanf(p, "%*s %d", &t.m_mtlID) != 1) {
                return false;
            }
            t.m_vtx1 = &m_vtxs.at(idx1);
            t.m_vtx2 = &m_vtxs.at(idx2);
            t.m_vtx3 = &m_vtxs.at(idx3);
            is.getline(buf, sizeof(buf));
            m_tris.push_back(t);
        }
        return !is.fail();
    }

    bool ASE::Geo::readMeshVtxTUV(istream& is) {
        Tuv t; char buf[512];

        is.getline(buf, sizeof(buf));
        while (!is.fail() && !strchr(buf, '}')) {
            if (sscanf(buf, "%*s %*d %f %f %f", &t.m_u, &t.m_v, &t.m_w) != 3) {
                return false;
            }
            t.m_v = 1.f - t.m_v; // invert to flip texture about v
            is.getline(buf, sizeof(buf));
            m_tuvs.push_back(t);
        }
        return !is.fail();
    }

    bool ASE::Geo::readMeshTriTUV(istream& is) {
        uint32_t n, a, b, c;
        char buf[512];

        is.getline(buf, sizeof(buf));
        while (!is.fail() && !strchr(buf, '}')) {
            if (sscanf(buf, "%*s %d %d %d %d", &n, &a, &b, &c) != 4) {
                return false;
            }
            Tri* tri = &m_tris.at(n);
            tri->m_tuv1 = &m_tuvs.at(a);
            tri->m_tuv2 = &m_tuvs.at(b);
            tri->m_tuv3 = &m_tuvs.at(c);
            is.getline(buf, sizeof(buf));
        }
        return !is.fail();
    }

    bool ASE::Geo::readMeshNrm(istream& is) {
        char buf[512];
        float x, y, z;
        uint32_t n;

        is.getline(buf, sizeof(buf));
        while (!is.fail() && !strchr(buf, '}')) {
            if (sscanf(buf, "%*s %d %f %f %f", &n, &x, &y, &z) != 4) {
                return false;
            }

            // note: normals are in model space
            Tri* tri = &m_tris.at(n);
            tri->m_nrmF.m_x = x;
            tri->m_nrmF.m_y = y;
            tri->m_nrmF.m_z = z;
            tri->m_nrmF.m_w = 1.f;
            Vector1x4* nrm1 = &tri->m_nrm1;
            Vector1x4* nrm2 = &tri->m_nrm2;
            Vector1x4* nrm3 = &tri->m_nrm3;
            nrm1->m_w = 1.f;
            nrm2->m_w = 1.f;
            nrm3->m_w = 1.f;

            is.getline(buf, sizeof(buf));
            if (is.fail() || sscanf(buf, "%*s %*d %f %f %f", &nrm1->m_x, &nrm1->m_y, &nrm1->m_z) != 3)
                return false;
            is.getline(buf, sizeof(buf));
            if (is.fail() || sscanf(buf, "%*s %*d %f %f %f", &nrm2->m_x, &nrm2->m_y, &nrm2->m_z) != 3)
                return false;
            is.getline(buf, sizeof(buf));
            if (is.fail() || sscanf(buf, "%*s %*d %f %f %f", &nrm3->m_x, &nrm3->m_y, &nrm3->m_z) != 3)
                return false;

            is.getline(buf, sizeof(buf));
        }
        return !is.fail();
    }

    ASE::Geo::Geo() : m_mtx(Matrix4x4::IDENTITY) {
        m_mtlID = UINT_MAX; // geometry has no material
    }

    ASE::Geo::~Geo() {
    }

    void ASE::Geo::calcBinormal(Vertex_P3N3B3T2& vtx1, const Vertex_P3N3B3T2& vtx2, const Vertex_P3N3B3T2& vtx3) const {
        Vector1x4 v12(vtx2.m_x - vtx1.m_x, vtx2.m_y - vtx1.m_y, vtx2.m_z - vtx1.m_z, 1.f); // edge v1 to v2
        Vector1x4 v13(vtx3.m_x - vtx1.m_x, vtx3.m_y - vtx1.m_y, vtx3.m_z - vtx1.m_z, 1.f); // edge v1 to v3
        Vector1x4 n(vtx1.m_nx, vtx1.m_ny, vtx2.m_nz, 1.f); // v1 normal
        Vector1x4 proj12 = v12 - ((n * v12) * n); // project v1 to v2 onto n and subtract from v1 to v2
        Vector1x4 proj13 = v13 - ((n * v13) * n); // project v1 to v3 onto n and subtract from v1 to v3

        float du12 = vtx2.m_u0 - vtx1.m_u0;
        float du13 = vtx3.m_u0 - vtx1.m_u0;
        float dv12 = vtx2.m_v0 - vtx1.m_v0;
        float dv13 = vtx3.m_v0 - vtx1.m_v0;

        if ((du13 * dv12) > (du12 * dv13)) {
            du13 = -du13;
            du12 = -du12;
        }

        Vector1x4 binormal =  proj12 * du13 - proj13 * du12;
        binormal.normalize(); // travels in dv direction
        vtx1.m_bx = binormal.m_x;
        vtx1.m_by = binormal.m_y;
        vtx1.m_bz = binormal.m_z;
    }

    template<class T> struct Less {
        bool operator()(const T* l, const T* r) const { return *l < *r; }
    };

    void ASE::Geo::addTrisTypeA(DrawableModel* model, const ASE::Mtl* mtl, uint32_t mtlID) const {
        typedef Vertex_P3N3 Vtx;

        typedef map<Vtx*, size_t, Less<Vtx> > Map;
        vector<Vtx>      vec;
        vector<uint16_t> idx;
        Map mpv;
        Vtx vtx;
        size_t j;

        pair<Map::iterator, bool> p;
        vec.reserve(m_tris.size() * 3); // musn't realloc during loop
        idx.reserve(m_tris.size() * 3);

        for (vector<Tri>::const_iterator i = m_tris.begin(); i < m_tris.end(); ++i) {
            if (mtlID == UINT_MAX || // all tris consist of this material or
                    i->m_mtlID == mtlID) { // check to see if tri is of this material
                vtx.m_x = i->m_vtx1->m_x;
                vtx.m_y = i->m_vtx1->m_y;
                vtx.m_z = i->m_vtx1->m_z;
                vtx.m_nx = i->m_nrm1.m_x;
                vtx.m_ny = i->m_nrm1.m_y;
                vtx.m_nz = i->m_nrm1.m_z;

                vec.push_back(vtx);
                j = vec.size() - 1;
                p = mpv.insert(Map::value_type(&vec[j], j));
                if (!p.second) {
                    vec.pop_back();
                    j = p.first->second;
                }
                idx.push_back((uint16_t)j);

                vtx.m_x = i->m_vtx2->m_x;
                vtx.m_y = i->m_vtx2->m_y;
                vtx.m_z = i->m_vtx2->m_z;
                vtx.m_nx = i->m_nrm2.m_x;
                vtx.m_ny = i->m_nrm2.m_y;
                vtx.m_nz = i->m_nrm2.m_z;

                vec.push_back(vtx);
                j = vec.size() - 1;
                p = mpv.insert(Map::value_type(&vec[j], j));
                if (!p.second) {
                    vec.pop_back();
                    j = p.first->second;
                }
                idx.push_back((uint16_t)j);

                vtx.m_x = i->m_vtx3->m_x;
                vtx.m_y = i->m_vtx3->m_y;
                vtx.m_z = i->m_vtx3->m_z;
                vtx.m_nx = i->m_nrm3.m_x;
                vtx.m_ny = i->m_nrm3.m_y;
                vtx.m_nz = i->m_nrm3.m_z;

                vec.push_back(vtx);
                j = vec.size() - 1;
                p = mpv.insert(Map::value_type(&vec[j], j));
                if (!p.second) {
                    vec.pop_back();
                    j = p.first->second;
                }
                idx.push_back((uint16_t)j);
            }
        }
        if (!vec.empty()) {
            if (mtl) {
                model->addDrawBatch(mtl->m_mtl, vec, idx);
            } else {
                Material* mat = Material::get("ASE Default");
                Shader* s = Shader::get("P3N3");
                mat->setShader(s);
                model->addDrawBatch(mat, vec, idx);
            }
        }
    }

    void ASE::Geo::addTrisTypeB(DrawableModel* model, const ASE::Mtl* mtl, uint32_t mtlID) const {
        typedef Vertex_P3N3T2 Vtx;

        typedef map<Vtx*, size_t, Less<Vtx> > Map;
        vector<Vtx>      vec;
        vector<uint16_t> idx;
        Map mpv;
        Vtx vtx;
        size_t j;

        pair<Map::iterator, bool> p;
        vec.reserve(m_tris.size() * 3); // must not realloc during loop
        idx.reserve(m_tris.size() * 3);

        for (vector<Tri>::const_iterator i = m_tris.begin(); i < m_tris.end(); ++i) {
            if (mtlID == UINT_MAX || // all triangles consist of this material or
                    i->m_mtlID == mtlID) { // check to see if tri is of this material
                vtx.m_x = i->m_vtx1->m_x;
                vtx.m_y = i->m_vtx1->m_y;
                vtx.m_z = i->m_vtx1->m_z;
                vtx.m_nx = i->m_nrm1.m_x;
                vtx.m_ny = i->m_nrm1.m_y;
                vtx.m_nz = i->m_nrm1.m_z;
                vtx.m_u0 = (i->m_tuv1->m_u - 0.5f - mtl->m_uOffset) * mtl->m_uTiling + 0.5f;
                vtx.m_v0 = (i->m_tuv1->m_v - 0.5f - mtl->m_vOffset) * mtl->m_vTiling + 0.5f;

                vec.push_back(vtx);
                j = vec.size() - 1;
                p = mpv.insert(Map::value_type(&vec[j], j));
                if (!p.second) {
                    vec.pop_back();
                    j = p.first->second;
                }
                idx.push_back((uint16_t)j);

                vtx.m_x = i->m_vtx2->m_x;
                vtx.m_y = i->m_vtx2->m_y;
                vtx.m_z = i->m_vtx2->m_z;
                vtx.m_nx = i->m_nrm2.m_x;
                vtx.m_ny = i->m_nrm2.m_y;
                vtx.m_nz = i->m_nrm2.m_z;
                vtx.m_u0 = (i->m_tuv2->m_u - 0.5f - mtl->m_uOffset) * mtl->m_uTiling + 0.5f;
                vtx.m_v0 = (i->m_tuv2->m_v - 0.5f - mtl->m_vOffset) * mtl->m_vTiling + 0.5f;

                vec.push_back(vtx);
                j = vec.size() - 1;
                p = mpv.insert(Map::value_type(&vec[j], j));
                if (!p.second) {
                    vec.pop_back();
                    j = p.first->second;
                }
                idx.push_back((uint16_t)j);

                vtx.m_x = i->m_vtx3->m_x;
                vtx.m_y = i->m_vtx3->m_y;
                vtx.m_z = i->m_vtx3->m_z;
                vtx.m_nx = i->m_nrm3.m_x;
                vtx.m_ny = i->m_nrm3.m_y;
                vtx.m_nz = i->m_nrm3.m_z;
                vtx.m_u0 = (i->m_tuv3->m_u - 0.5f - mtl->m_uOffset) * mtl->m_uTiling + 0.5f;
                vtx.m_v0 = (i->m_tuv3->m_v - 0.5f - mtl->m_vOffset) * mtl->m_vTiling + 0.5f;

                vec.push_back(vtx);
                j = vec.size() - 1;
                p = mpv.insert(Map::value_type(&vec[j], j));
                if (!p.second) {
                    vec.pop_back();
                    j = p.first->second;
                }
                idx.push_back((uint16_t)j);
            }
        }
        if (!vec.empty()) {
            model->addDrawBatch(mtl->m_mtl, vec, idx);
        }
    }

    void ASE::Geo::addTrisTypeC(DrawableModel* model, const ASE::Mtl* mtl, uint32_t mtlID) const {
        typedef Vertex_P3N3B3T2 Vtx;

        typedef map<Vtx*, size_t, Less<Vtx> > Map;
        vector<Vtx>      vec;
        vector<uint16_t> idx;
        Vtx vtx1;
        Vtx vtx2;
        Vtx vtx3;
        size_t j;
        Map mpv;

        pair<Map::iterator, bool> p;
        vec.reserve(m_tris.size() * 3); // must not realloc during loop
        idx.reserve(m_tris.size() * 3);

        for (vector<Tri>::const_iterator i = m_tris.begin(); i < m_tris.end(); ++i) {
            if (mtlID == UINT_MAX || // all triangles consist of this material or
                    i->m_mtlID == mtlID) { // check to see if tri is of this material
                vtx1.m_x = i->m_vtx1->m_x;
                vtx1.m_y = i->m_vtx1->m_y;
                vtx1.m_z = i->m_vtx1->m_z;
                vtx1.m_nx = i->m_nrm1.m_x;
                vtx1.m_ny = i->m_nrm1.m_y;
                vtx1.m_nz = i->m_nrm1.m_z;
                vtx1.m_u0 = (i->m_tuv1->m_u - 0.5f - mtl->m_uOffset) * mtl->m_uTiling + 0.5f;
                vtx1.m_v0 = (i->m_tuv1->m_v - 0.5f - mtl->m_vOffset) * mtl->m_vTiling + 0.5f;

                vtx2.m_x = i->m_vtx2->m_x;
                vtx2.m_y = i->m_vtx2->m_y;
                vtx2.m_z = i->m_vtx2->m_z;
                vtx2.m_nx = i->m_nrm2.m_x;
                vtx2.m_ny = i->m_nrm2.m_y;
                vtx2.m_nz = i->m_nrm2.m_z;
                vtx2.m_u0 = (i->m_tuv2->m_u - 0.5f - mtl->m_uOffset) * mtl->m_uTiling + 0.5f;
                vtx2.m_v0 = (i->m_tuv2->m_v - 0.5f - mtl->m_vOffset) * mtl->m_vTiling + 0.5f;

                vtx3.m_x = i->m_vtx3->m_x;
                vtx3.m_y = i->m_vtx3->m_y;
                vtx3.m_z = i->m_vtx3->m_z;
                vtx3.m_nx = i->m_nrm3.m_x;
                vtx3.m_ny = i->m_nrm3.m_y;
                vtx3.m_nz = i->m_nrm3.m_z;
                vtx3.m_u0 = (i->m_tuv3->m_u - 0.5f - mtl->m_uOffset) * mtl->m_uTiling + 0.5f;
                vtx3.m_v0 = (i->m_tuv3->m_v - 0.5f - mtl->m_vOffset) * mtl->m_vTiling + 0.5f;

                calcBinormal(vtx1, vtx2, vtx3);
                calcBinormal(vtx2, vtx3, vtx1);
                calcBinormal(vtx3, vtx1, vtx2);

                vec.push_back(vtx1);
                j = vec.size() - 1;
                p = mpv.insert(Map::value_type(&vec[j], j));
                if (!p.second) {
                    vec.pop_back();
                    j = p.first->second;
                }
                idx.push_back((uint16_t)j);

                vec.push_back(vtx2);
                j = vec.size() - 1;
                p = mpv.insert(Map::value_type(&vec[j], j));
                if (!p.second) {
                    vec.pop_back();
                    j = p.first->second;
                }
                idx.push_back((uint16_t)j);

                vec.push_back(vtx3);
                j = vec.size() - 1;
                p = mpv.insert(Map::value_type(&vec[j], j));
                if (!p.second) {
                    vec.pop_back();
                    j = p.first->second;
                }
                idx.push_back((uint16_t)j);
            }
        }
        if (!vec.empty()) {
            model->addDrawBatch(mtl->m_mtl, vec, idx);
        }
    }

    void ASE::Geo::addStdMtlTris(DrawableModel* model, const ASE::Mtl* mtl, uint32_t mtlID) const {
        if (mtl->m_mtl->getTexture(0)) { // has diffuse texture map
            if (mtl->m_mtl->getTexture(1)) { // has normal texture map
                addTrisTypeC(model, mtl, mtlID);
            } else {
                addTrisTypeB(model, mtl, mtlID);
            }
        } else {
            addTrisTypeA(model, mtl, mtlID);
        }
    }

    void ASE::Geo::addMulMtlTris(DrawableModel* model, const ASE::Mtl* mtl) const {
        assert(!mtl->m_mtls.empty());
        for (uint32_t i = 0; i < mtl->m_mtls.size(); ++i) { // for each sub material
            addStdMtlTris(model, mtl->m_mtls[i], i); // collect relevant triangles
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    ASE::Cam::Cam() : m_camMtx(Matrix4x4::IDENTITY), m_tgtMtx(Matrix4x4::IDENTITY) {
        m_clipN = CAMERA_DEFAULT_N_CLIP_DISTANCE;
        m_clipF = CAMERA_DEFAULT_F_CLIP_DISTANCE;
        m_fov = DegressToRadians(45.0f);
    }

    bool ASE::Cam::read(istream& is) {
        char *p, *q, buf[512];
        uint32_t nodeTM = 0;

        is.getline(buf, sizeof(buf));
        while (!is.fail()) {
            p = strtok(buf, " \t\n");
            q = strlen(p) + p + 1;

            if (!strcmp(p, "*CAMERA_SETTINGS"   )) {
                if (!readset(is))
                    return false;
            } else if (!strcmp(p, "*NODE_TM"    )) {
                switch (nodeTM) {
                case 0:
                    if (!readMtx(is, m_camMtx)) {
                        return false;
                    }
                    ++nodeTM; // now equals 1
                    break;
                case 1:
                    if (!readMtx(is, m_tgtMtx)) {
                        return false;
                    }
                    ++nodeTM; // now equals 2
                    break;
                default:
                    return false;
                }
            } else if (strchr(p, '}')) {
                break;
            } else if (strchr(q, '{')) {
                if (!skipBlock(is))
                    return false;
            }
            is.getline(buf, sizeof(buf));
        }

        bool ok = !is.fail();
        if (ok) { // 3DS-MAX cameras use a y up vector and a -ve z look at vector
            // whereas the engine uses a z up vector and a +ve y look at vector
            // therefore we need to exchange rows y and z and negate row y
            Vector1x4 x(m_camMtx.m_11,
                        m_camMtx.m_12,
                        m_camMtx.m_13,
                        1.f);
            Vector1x4 z(m_camMtx.m_21,
                        m_camMtx.m_22,
                        m_camMtx.m_23,
                        1.f);
            Vector1x4 y(m_camMtx.m_21 = -m_camMtx.m_31,
                        m_camMtx.m_22 = -m_camMtx.m_32,
                        m_camMtx.m_23 = -m_camMtx.m_33,
                        1.f);

            m_camMtx.m_31 = z.m_x;
            m_camMtx.m_32 = z.m_y;
            m_camMtx.m_33 = z.m_z;

            if (y.m_z < +0.9f && y.m_z > -0.9f) {
                // align target frame y axis with camera y-z plane
                y.m_z = +0.0f;
                y.normalize();
                z = Vector1x4(0.0f, 0.0f, 1.0f, 1.f);
                x = y ^ z;
            } else {
                // align target frame x axis with camera z-x plane
                x.m_z = +0.0f;
                x.normalize();
                z = Vector1x4(0.0f, 0.0f, 1.0f, 1.f);
                y = z ^ x;
            }
            m_tgtMtx.m_11 = x.m_x; m_tgtMtx.m_12 = x.m_y; m_tgtMtx.m_13 = x.m_z;
            m_tgtMtx.m_21 = y.m_x; m_tgtMtx.m_22 = y.m_y; m_tgtMtx.m_23 = y.m_z;
            m_tgtMtx.m_31 = z.m_x; m_tgtMtx.m_32 = z.m_y; m_tgtMtx.m_33 = z.m_z;

            if (nodeTM == 1) { // free camera with no target frame so make one
                // create target frame position by projecting camera
                // position down y look at axis
                m_tgtMtx.m_41 = m_camMtx.m_41 + 3.0f * m_camMtx.m_21;
                m_tgtMtx.m_42 = m_camMtx.m_42 + 3.0f * m_camMtx.m_22;
                m_tgtMtx.m_43 = m_camMtx.m_43 + 3.0f * m_camMtx.m_23;
            }

            // set some limits for near and far clip planes
            if (m_clipN < CAMERA_DEFAULT_N_CLIP_DISTANCE) {
                m_clipN = CAMERA_DEFAULT_N_CLIP_DISTANCE;
            }
            if (m_clipN > m_clipF - 10.0f) {
                m_clipN = CAMERA_DEFAULT_N_CLIP_DISTANCE;
                m_clipF = CAMERA_DEFAULT_F_CLIP_DISTANCE;
            }
        }
        return ok;
    }

    bool ASE::Cam::readset(istream& is) {
        char *p, *q, buf[512];

        is.getline(buf, sizeof(buf));
        while (!is.fail()) {
            p = strtok(buf, " \t\n");
            q = strlen(p) + p + 1;

            if (!strcmp(p, "*CAMERA_NEAR" )) {
                if (sscanf(q, "%f", &m_clipN) != 1)
                    return false;
            } else if (!strcmp(p, "*CAMERA_FAR")) {
                if (sscanf(q, "%f", &m_clipF) != 1)
                    return false;
            } else if (!strcmp(p, "*CAMERA_FOV")) {
                if (sscanf(q, "%f", &m_fov) != 1)
                    return false;
            } else if (strchr(p, '}')) {
                break;
            } else if (strchr(q, '{')) {
                if (!skipBlock(is))
                    return false;
            }
            is.getline(buf, sizeof(buf));
        }
        return !is.fail();
    }

    bool ASE::Cam::readMtx(istream& is, Matrix4x4& mtx) {
        char *p, *q, buf[512];

        is.getline(buf, sizeof(buf));
        while (!is.fail()) {
            p = strtok(buf, " \t\n");
            q = strlen(p) + p + 1;

            if (!strcmp(p, "*TM_ROW0")) {
                if (sscanf(q, "%f %f %f", &mtx.m_11, &mtx.m_12, &mtx.m_13) != 3)
                    return false;
            } else if (!strcmp(p, "*TM_ROW1")) {
                if (sscanf(q, "%f %f %f", &mtx.m_21, &mtx.m_22, &mtx.m_23) != 3)
                    return false;
            } else if (!strcmp(p, "*TM_ROW2")) {
                if (sscanf(q, "%f %f %f", &mtx.m_31, &mtx.m_32, &mtx.m_33) != 3)
                    return false;
            } else if (!strcmp(p, "*TM_ROW3")) {
                if (sscanf(q, "%f %f %f", &mtx.m_41, &mtx.m_42, &mtx.m_43) != 3)
                    return false;
            } else if (strchr(p, '}')) {
                break;
            } else if (strchr(q, '{')) {
                if (!skipBlock(is))
                    return false;
            }
            is.getline(buf, sizeof(buf));
        }
        return !is.fail();
    }

}
