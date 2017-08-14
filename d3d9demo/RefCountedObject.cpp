#include "BasicTypes.h"

namespace eve {
	RefCountedObject::RefCountedObject() {
		m_refCount = 1;
	}
	RefCountedObject::~RefCountedObject() {
		EVE_ASSERT(m_refCount == 0);
	}

	void RefCountedObject::addRef() {
		++m_refCount;
	}
	void RefCountedObject::rmvRef() {
		EVE_ASSERT(m_refCount > 0);
		if (--m_refCount == 0) {
			delete this;
		}
	}
}
