//
// Created by notoraptor on 29/12/2017.
//

#ifndef SRC_CIGMAR_RLL_HPP
#define SRC_CIGMAR_RLL_HPP

/** RLL: Runtime Library Loading. **/

#ifdef WIN32
#include <libraries/dlfcn-win32/dlfcn.h>
#else
#include <dlfcn.h>
#endif
#include <cigmar/exception/Exception.hpp>

namespace cigmar {
	class RLL {
		void *handler;
		String name;
	public:
		explicit RLL(const char *libraryPath) : handler(nullptr), name(libraryPath) {
			handler = dlopen(libraryPath, RTLD_NOW);
			if (!handler)
				throw Exception("Error loading runtime library ", libraryPath, ". Error: ", dlerror());
		}
		explicit RLL(const String& libraryPath): RLL(libraryPath.cstring()) {}
		~RLL() {
			if (handler)
				std:: cerr << "Freeing rll: " << name << std::endl;
			if (handler && dlclose(handler) != 0)
				std::cerr << "Error closing runtime library. Error: " << dlerror();
		}
		const String& getLoadedPath() const { return name; }
		void* get(const char* functionName) const {
			void* functionPointer = dlsym(handler, functionName);
			if (!functionPointer)
				throw Exception("Error getting runtime loaded function. Error: ", dlerror());
			return functionPointer;
		}
	};
}

#endif //SRC_CIGMAR_RLL_HPP
