//
// Created by notoraptor on 04/03/2018.
//

#ifndef SRC_CIGMAR_STRING_STREAMS_HPP
#define SRC_CIGMAR_STRING_STREAMS_HPP

#include <ostream>
#include <sstream>
#include <cigmar/utils/trait_utils.hpp>
#include <cigmar/unicode.hpp>

namespace cigmar {
	namespace special_classes {
		struct AutoStream {
			template<typename C, typename E>
			static void print(std::basic_ostream<C>& o, const E& value) {
				o << value;
			}
			template <typename C>
			static void print(std::basic_ostream<C>& o, bool value) {
				o << (value ? "true" : "false");
			}
			template <typename C>
			static void print(std::basic_ostream<C>& o, const wchar_t* s) {
				unicode::convert(s, o);
			}
			template <typename C>
			static void print(std::basic_ostream<C>& o, const char16_t* s) {
				unicode::convert(s, o);
			}
			template <typename C>
			static void print(std::basic_ostream<C>& o, const char32_t* s) {
				unicode::convert(s, o);
			}
		};
		struct MethodToStream {
			template <typename C, typename T>
			static void print(std::basic_ostream<C>& o, const T& value) {
				value.toStream(o);
			}
		};
		struct DefaultStream {
			template<typename C, typename E>
			static void print(std::basic_ostream<C>& o, const E& value) {
				o << "(@" << (void*)(&value) << ')';
			}
		};
		template <typename T> struct Streamer {
			const T& value;
			explicit Streamer(const T& v): value(v) {};
		};

		template <typename C, typename T>
		std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, const special_classes::Streamer<T>& streamer) {
			typedef typename std::conditional<
					is_directly_streamable<T>{},
					special_classes::AutoStream,
					typename std::conditional<
							has_method_toStream<T>{},
							special_classes::MethodToStream,
							special_classes::DefaultStream
					>::type
			>::type streamer_t;
			streamer_t::print(o, streamer.value);
			return o;
		}
	}
	template <typename T> Streamer<T> streamer(const T& v) {
		return Streamer<T>(v);
	}

	namespace special_functions {
		template <typename C>
		void writeElement(std::basic_ostream<C>& o) {}
		template <typename C, typename T, typename... Args>
		void writeElement(std::basic_ostream<C>& o, const T& variable, Args&&... args) {
			o << special_classes::streamer(variable);
			writeElement(o, std::forward<Args>(args)...);
		};
		template <typename C>
		void printElement(std::basic_ostream<C>& o) {}
		template <typename C, typename T, typename... Args>
		void printElement(std::basic_ostream<C>& o, const T& variable, Args&&... args) {
			o << ' ' << special_classes::streamer(variable);
			printElement(o, std::forward<Args>(args)...);
		};
		template <typename C>
		void printFirstElement(std::basic_ostream<C>& o) {}
		template <typename C, typename T, typename... Args>
		void printFirstElement(std::basic_ostream<C>& o, const T& variable, Args&&... args) {
			o << special_classes::streamer(variable);
			printElement(o, std::forward<Args>(args)...);
		};
	}
	template <typename Character>
	struct standard_string {
		typedef std::basic_string<Character> string_t;
		string_t& member;
		explicit standard_string(string_t& m): member(m) {}

		void write() {}
		template<typename... Args>
		void write(const Character* str, Args&&... args) {
			member += str;
			write(std::forward<Args>(args)...);
		}
		template<typename OtherCharacter, typename... Args>
		void write(const OtherCharacter* s, Args&&... args) {
			ASSERT_CHARTYPE(OtherCharacter);
			unicode::convert(s, member);
			write(std::forward<Args>(args)...);
		}
		template<typename T, typename... Args>
		void write(T variable, Args&&... args) {
			std::basic_ostringstream<Character> o;
			o << streamer(variable);
			member += o.str();
			write(std::forward<Args>(args)...);
		}

		void print() {}
		template<typename... Args>
		void print(const Character* s, Args&&... args) {
			member += s;
			printNext(std::forward<Args>(args)...);
		}
		template<typename OtherCharacter, typename... Args>
		void print(const OtherCharacter* s, Args&&... args) {
			ASSERT_CHARTYPE(OtherCharacter);
			unicode::convert(s, member);
			printNext(args...);
		}
		template<typename T, typename... Args>
		void print(T variable, Args&&... args) {
			std::basic_ostringstream<Character> o;
			o << streamer(variable);
			member += o.str();
			printNext(std::forward<Args>(args)...);
		}

		void printNext() {}
		template<typename... Args>
		void printNext(const Character* s, Args&&... args) {
			member += ' ';
			member += s;
			printNext(std::forward<Args>(args)...);
		}
		template<typename OtherCharacter, typename... Args>
		void printNext(const OtherCharacter* s, Args&&... args) {
			ASSERT_CHARTYPE(OtherCharacter);
			member += ' ';
			unicode::convert(s, member);
			printNext(std::forward<Args>(args)...);
		}
		template<typename T, typename... Args>
		void printNext(T variable, Args&&... args) {
			std::basic_ostringstream<Character> o;
			o << streamer(variable);
			member += ' ';
			member += o.str();
			printNext(std::forward<Args>(args)...);
		}
	};
}
#endif //SRC_CIGMAR_STRING_STREAMS_HPP
