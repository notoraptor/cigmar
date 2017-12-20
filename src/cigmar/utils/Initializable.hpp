//
// Created by notoraptor on 17-11-22.
//

#ifndef SRC_CIGMAR_INITIALIZABLE_HPP
#define SRC_CIGMAR_INITIALIZABLE_HPP


#include <utility>
#include <cigmar/exception/Exception.hpp>

namespace cigmar {
	template<typename T>
	class Initializable {
	private:
		T content;
		bool initialized;
		void check() const {
			if (!initialized)
				throw Exception("Object not initialized.");
		}
	public:
		Initializable(): content(), initialized(false) {}
		Initializable(const T& other): content(other), initialized(true) {}
		Initializable(T&& other) noexcept : content(std::move(other)), initialized(true) {}
		template<typename... Args>
		explicit Initializable(Args&&... args): content(std::forward<Args>(args)...), initialized(true) {}
		template<typename... Args>
		void init(Args&&... args) {
			if (!initialized) {
				content.init(std::forward<Args>(args)...);
				initialized = true;
			}
		}
		Initializable& operator=(const T& other) {
			content = other;
			initialized = true;
			return *this;
		}
		Initializable& operator=(T&& other) {
			content = std::move(other);
			initialized = true;
			return *this;
		}
		explicit operator bool() const {return initialized;}
		T* operator->() {
			check();
			return &content;
		}
		const T* operator->() const {
			check();
			return &content;
		}
		T& operator*() {
			check();
			return content;
		}
		const T& operator*() const {
			check();
			return content;
		}
	};
}

#endif //SRC_CIGMAR_INITIALIZABLE_HPP
