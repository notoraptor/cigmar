#ifndef CIGMAR_STD
#define CIGMAR_STD

#include <cstring>
#include <string>
#include <vector>

namespace cigmar {

inline void std_string_replace_inplace(std::string& s, const char* from, const char* to) {
	size_t from_len = strlen(from);
	size_t to_len = strlen(to);
	size_t pos = 0;
	do {
		pos = s.find(from, pos);
		if (pos != std::string::npos) {
			s.replace(pos, from_len, to);
			pos += to_len;
		}
	} while(pos != std::string::npos);
}

inline void std_string_split(const std::string& s, const char* delimiter, std::vector<std::string>& v) {
	size_t sep_len = strlen(delimiter);
	size_t piece_start = 0;
	size_t next_start = 0;
	do {
		next_start = s.find(delimiter, piece_start);
		v.push_back(std::move(s.substr(piece_start, next_start - piece_start)));
		piece_start = next_start + sep_len;
	} while (next_start != std::string::npos);
}

}

#endif
