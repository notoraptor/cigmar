//
// Created by notoraptor on 31/10/2017.
//

#ifndef CIGMAR_EXCEPTION_HPP
#define CIGMAR_EXCEPTION_HPP

#include <exception>
#include <cigmar/classes/String.hpp>

namespace cigmar {
    class Exception: public std::exception {
        String m_message;
    public:
        Exception(): std::exception(), m_message() {}
        explicit Exception(const char* msg): std::exception(), m_message(msg) {}
        explicit Exception(const String& msg): std::exception(), m_message(msg) {}
        explicit Exception(const std::exception& exc): std::exception(), m_message(exc.what()) {}
        Exception(const Exception& other) noexcept: std::exception(), m_message(other.m_message) {}
        template<typename... Args>
        explicit Exception(Args... args): std::exception(), m_message(String::write(args...)) {}
        Exception& operator=(const Exception& other) {
            m_message = other.m_message;
            return *this;
        }
        const char* what() const noexcept override {return m_message.cstring();}
		const String& message() const {return m_message;}
    };
	template <typename C>
	std::basic_ostream<C>& operator<<(std::basic_ostream<C>& o, const Exception& e) {
		o << e.message();
		return o;
	}
}

#endif //CIGMAR_EXCEPTION_HPP
