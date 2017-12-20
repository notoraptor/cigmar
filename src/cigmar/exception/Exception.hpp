//
// Created by notoraptor on 31/10/2017.
//

#ifndef CIGMAR_EXCEPTION_HPP
#define CIGMAR_EXCEPTION_HPP

#include <exception>
#include <cigmar/interfaces/Streamable.hpp>
#include <cigmar/classes/String.hpp>

namespace cigmar {
    class Exception: public std::exception, public Streamable {
    private:
        String message;
    public:
        Exception(): std::exception(), message() {}
        explicit Exception(const char* msg): std::exception(), message(msg) {}
        explicit Exception(const String& msg): std::exception(), message(msg) {}
        explicit Exception(const std::exception& exc): std::exception(), message(exc.what()) {}
        Exception(const Exception& other) noexcept: std::exception(), message(other.message) {}
        template<typename... Args>
        explicit Exception(Args... args): std::exception(), message(String::write(args...)) {}
        Exception& operator=(const Exception& other) {
            message = other.message;
            return *this;
        }
        const char* what() const noexcept override {return message.cstring();}
        void toStream(std::ostream& o) const override {o << message;}
    };
}

#endif //CIGMAR_EXCEPTION_HPP
