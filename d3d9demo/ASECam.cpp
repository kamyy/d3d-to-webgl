#include "BasicTypes.h"
#include "ASE.h"

namespace eve {
	ASE::Cam::Cam() : m_camMtx(Matrix4x4::IDENTITY), m_tgtMtx(Matrix4x4::IDENTITY) {
		m_clipN = CAMERA_DEFAULT_N_CLIP_DISTANCE;
		m_clipF = CAMERA_DEFAULT_F_CLIP_DISTANCE;
		m_fov = D3DXToRadian(45.0f);
	}
	bool ASE::Cam::read(istream& is) {
		char *p, *q, buf[512];
		u32 nodeTM = 0;

		is.getline(buf, sizeof(buf));
		while (!is.fail()) {
			p = strtok(buf, " \t\n");
			q = strlen(p) + p + 1;

			if (!strcmp(p, "*CAMERA_SETTINGS"	)) {
				if (!readset(is))
					return false;
			} else if (!strcmp(p, "*NODE_TM"	)) {
				switch (nodeTM) {
				case 0:
					if (!readMtx(is, m_camMtx))
						return false;
					++nodeTM; // now equals 1
					break;
				case 1:
					if (!readMtx(is, m_tgtMtx))
						return false;
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