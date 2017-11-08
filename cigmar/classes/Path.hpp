#ifndef CIGMAR_PATH_HPP_INCLUDED
#define CIGMAR_PATH_HPP_INCLUDED

#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/interfaces/Comparable.hpp>
#include <cigmar/classes/Exception.hpp>
#include <cigmar/filesystem.hpp>

#define PATH_STATE_IS_FILE 0
#define PATH_STATE_IS_DIRECTORY 1
#define PATH_STATE_HAS_EXTENSION 2
#define PATH_STATE_IS_ABSOLUTE 3
#define PATH_STATE_IS_NORMALIZED 4
#define PATH_STATE_IS_RESOLVED 5
#define PATH_STATE_IS_ROOT 6
#define PATH_STATE_COUNT 7
#define PATH_INFO_UNDEFINED (-1)
#define PATH_INFO_FALSE 0
#define PATH_INFO_TRUE 1

namespace cigmar::sys {

	class Path: public Streamable, public Comparable<Path> {
	private:
		class PathState {
		private:
			int state;
		public:
			PathState(): state(PATH_INFO_UNDEFINED) {}
			PathState& operator=(bool value) {
				state = value ? PATH_INFO_TRUE : PATH_INFO_FALSE;
				return *this;
			}
			bool operator~() const {return state == PATH_INFO_UNDEFINED;}
			explicit operator bool() const {return state == PATH_INFO_TRUE;}
			bool operator!() const {return state == PATH_INFO_FALSE;}
		};
	private:
		String m_path;
		PathState states[PATH_STATE_COUNT];
	private:
		void init() {
			if (path::isRoot((const char*)m_path)) {
				states[PATH_STATE_IS_FILE] = false;
				states[PATH_STATE_IS_DIRECTORY] = true;
				states[PATH_STATE_HAS_EXTENSION] = false;
				states[PATH_STATE_IS_ABSOLUTE] = true;
				states[PATH_STATE_IS_NORMALIZED] = true;
				states[PATH_STATE_IS_RESOLVED] = true;
				states[PATH_STATE_IS_ROOT] = true;
			} else if (isSpecialFolder()) {
				states[PATH_STATE_IS_FILE] = false;
				states[PATH_STATE_IS_DIRECTORY] = true;
				states[PATH_STATE_HAS_EXTENSION] = false;
				states[PATH_STATE_IS_ABSOLUTE] = false;
				states[PATH_STATE_IS_NORMALIZED] = true;
				states[PATH_STATE_IS_RESOLVED] = true;
				states[PATH_STATE_IS_ROOT] = false;
			} else {
				m_path.rtrim(path::separator);
			}
		}
		bool isSpecialFolder() const {
			return m_path == "." || m_path == "..";
		}
	public:
		explicit Path(const char* p): m_path(p), states() {init();};
		explicit Path(const String& p): m_path(p), states() {init();};
		explicit Path(String&& p): m_path(std::move(p)), states() {init();};
		Path(const Path& parent, const char* p): m_path(path::join(parent, p)), states() {init();};
		Path(const Path& parent, const String& p): m_path(path::join(parent, p)), states() {init();};
		Path(const Path& p) = default;
		Path(Path&& p) = default;
		Path& operator=(const Path&) = default;
		Path& operator=(Path&&) = default;
		bool exists() {
			if (~states[PATH_STATE_IS_FILE]) {
				PathState& s = states[PATH_STATE_IS_FILE];
				states[PATH_STATE_IS_FILE] = path::isFile((const char*)m_path);
			}
			if (~states[PATH_STATE_IS_DIRECTORY]) {
				states[PATH_STATE_IS_DIRECTORY] = path::isDirectory((const char*)m_path);
			}
			return states[PATH_STATE_IS_FILE] || states[PATH_STATE_IS_DIRECTORY];
		};
		bool isFile() {
			if (~states[PATH_STATE_IS_FILE]) {
				states[PATH_STATE_IS_FILE] = path::isFile((const char*)m_path);
			}
			return (bool)states[PATH_STATE_IS_FILE];
		};
		bool isDirectory() {
			if (~states[PATH_STATE_IS_DIRECTORY]) {
				states[PATH_STATE_IS_DIRECTORY] = path::isDirectory((const char*)m_path);
			}
			return (bool)states[PATH_STATE_IS_DIRECTORY];
		};
		bool hasExtension() {
			if (~states[PATH_STATE_HAS_EXTENSION]) {
				states[PATH_STATE_HAS_EXTENSION] = false;
				pos_t posLastPoint = m_path.lastIndexOf('.');
				if (posLastPoint) {
					pos_t posLastSeparator = m_path.lastIndexOf(path::separator);
					states[PATH_STATE_HAS_EXTENSION] = !posLastSeparator || (posLastSeparator < posLastPoint);
				}
			}
			return (bool)states[PATH_STATE_HAS_EXTENSION];
		};
		Path& absolute() {
			if (~states[PATH_STATE_IS_ABSOLUTE] || !states[PATH_STATE_IS_ABSOLUTE]) {
				m_path = sys::path::absolute((const char*)m_path);
				states[PATH_STATE_IS_ABSOLUTE] = true;
				states[PATH_STATE_IS_RESOLVED] = true;
				states[PATH_STATE_IS_NORMALIZED] = true;
			}
			return *this;
		};
		Path& normalized() {
			if (~states[PATH_STATE_IS_NORMALIZED] || !states[PATH_STATE_IS_NORMALIZED]) {
				m_path = sys::path::norm((const char*)m_path);
				states[PATH_STATE_IS_NORMALIZED] = true;
			}
			return *this;
		};
		Path& resolve() {
			if (~states[PATH_STATE_IS_RESOLVED] || !states[PATH_STATE_IS_RESOLVED]) {
				m_path = path::resolve((const char*)m_path);
				states[PATH_STATE_IS_RESOLVED] = true;
				states[PATH_STATE_IS_NORMALIZED] = true;
			}
			return *this;
		};
		Path parent() const {
			if (states[PATH_STATE_IS_ROOT])
				throw Exception("A filesystem root does not have parent directory.");
			pos_t posLastSeparator = m_path.lastIndexOf(path::separator);
			if (!posLastSeparator)
				return Path(".");
			return Path(String(m_path, 0, posLastSeparator));
		};
		String basename() {
			if (states[PATH_STATE_IS_ROOT]) {
				// On Unix, return an empty string.
				// On Windows, return the disk letter.
				return String(m_path, 0, 1).upper();
			}
			if (isSpecialFolder())
				absolute();
			pos_t posLastSeparator = m_path.lastIndexOf(path::separator);
			if (!posLastSeparator)
				return m_path;
			return String(m_path, (size_t)posLastSeparator + 1);
		};
		String filename() {
			String baseName = basename();
			pos_t posLastPoint = baseName.lastIndexOf('.');
			if (!posLastPoint)
				return baseName;
			return String(baseName, (size_t)0, (size_t)posLastPoint);
		};
		String extension() {
			if (hasExtension())
				return String(m_path, m_path.lastIndexOf('.') + 1);
			return String("");
		};
		Dir directory() {
			if (!isDirectory())
				throw Exception("Cannot convert a non-directory path to a directory object.");
			return Dir((const char*)m_path);
		};
		void toStream(std::ostream& o) const override {
			o << m_path;
		};
		int compare(const Path& other) const override {
			return Path(*this).absolute() == Path(other).absolute();
		}
	};

}

#undef PATH_STATE_IS_FILE
#undef PATH_STATE_IS_DIRECTORY
#undef PATH_STATE_HAS_EXTENSION
#undef PATH_STATE_IS_ABSOLUTE
#undef PATH_STATE_IS_NORMALIZED
#undef PATH_STATE_IS_RESOLVED
#undef PATH_STATE_IS_ROOT
#undef PATH_STATE_COUNT
#undef PATH_INFO_UNDEFINED
#undef PATH_INFO_FALSE
#undef PATH_INFO_TRUE


#endif // CIGMAR_PATH_HPP_INCLUDED
