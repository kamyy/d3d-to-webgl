#pragma once
#ifndef RefCountedObject_h
#define RefCountedObject_h

namespace eve {
	class RefCountedObject {
	public:
		RefCountedObject();
		virtual ~RefCountedObject();

		void addRef();
		void rmvRef();

	private:
		i32 m_refCount;

		RefCountedObject(const RefCountedObject&);
		RefCountedObject operator=(const RefCountedObject&);
	};
}

#endif
