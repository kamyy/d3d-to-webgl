#include "BasicTypes.h"
#include "ASE.h"

namespace eve {
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
			} else if (!strcmp(p, "*SCENE_TICKSPERFRAME" )) {
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
}