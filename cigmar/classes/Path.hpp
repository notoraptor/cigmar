#ifndef CIGMAR_PATH_HPP_INCLUDED
#define CIGMAR_PATH_HPP_INCLUDED

#include <bitset>
#include <cigmar/filesystem.hpp>

namespace cigmar::sys {

class Path {
private:
	String path;
public:
	Path(const char* p);
	Path(const String& p);
	Path(const Path& parent, const char* p);
	Path(const Path& parent, const String& p);
	Path(const Path& p);
	Path(Path&& p);
	bool exists() const;
	bool isFile() const;
	bool isDirectory() const;
	bool hasExtension() const;
	Path absolute() const;
	Path normalized() const;
	Path parent() const;
	String filename() const;
	String basename() const;
	String extension() const;
	operator const String&() const;
	Path& resolve();
};

}


#endif // CIGMAR_PATH_HPP_INCLUDED
