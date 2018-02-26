#ifndef CIGMAR_STACK
#define CIGMAR_STACK

#include <forward_list>
#include <initializer_list>
#include <cigmar/symbols.hpp>

namespace cigmar {
	template<typename T>
	class Stack {
		template<typename E>
		struct StackElementPusher {
			void push(Stack& stack, const E& element) {
				stack.push(element);
			};
		};
		template<typename E>
		struct StackIterablePusher {
			void push(Stack& stack, const E& iterable) {
				stack.pushAll(std::begin(iterable), std::end(iterable));
			};
		};
	public:
		typedef std::forward_list<T> stack_type;
		typedef typename std::forward_list<T>::iterator iterator_t;
		typedef typename std::forward_list<T>::const_iterator const_iterator_t;
	private:
		stack_type content;
		size_t length;
	public:
		Stack() : content(), length(0) {}
		template <typename Iterator>
		Stack(Iterator firstIncluded, Iterator lastExcluded): Stack() {pushAll(firstIncluded, lastExcluded);}
		Stack(std::initializer_list<T> il): Stack() {pushAll(std::begin(il), std::end(il));}
		Stack(const Stack& other): content(other.content), length(other.length) {}
		Stack(Stack&&) noexcept = default;

		size_t size() const {return length;}

		Stack& push(const T& val) {
			content.push_front(val);
			++length;
			return *this;
		}
		Stack& push(T&& val) {
			content.push_front(std::move(val));
			++length;
			return *this;
		}
		template<typename Iterator>
		Stack& pushAll(Iterator iterator, Iterator endIterator) {
			while (iterator != endIterator) {
				content.push_front(*iterator);
				++iterator;
				++length;
			}
			return *this;
		}
		Stack& pop() {
			content.pop_front();
			--length;
			return *this;
		}
		Stack& swap(Stack& other) {
			content.swap(other.content);
			std::swap(length, other.length);
			return *this;
		}
		Stack& clear() {
			content.clear();
			length = 0;
			return *this;
		}
		T& front() {return content.front();}
		const T& front() const {return content.front();}

		iterator_t begin() {return content.begin();}
		iterator_t end() {return content.end();}
		const_iterator_t begin() const {return content.begin();}
		const_iterator_t end() const {return content.end();}

		Stack& operator=(const Stack& other) {
			content = other.content;
			length = other.length;
			return *this;
		}
		Stack& operator=(Stack&&) noexcept = default;
		Stack& operator=(std::initializer_list<T> il) {
			content.clear();
			length = 0;
			pushAll(std::begin(il), std::end(il));
			return *this;
		}
		template<typename C>
		Stack& operator=(const C& arr) {
			static_assert(is_iterable<C>::value, "Only an iterable can be affected to a stack.");
			content.clear();
			length = 0;
			pushAll(std::begin(arr), std::end(arr));
			return *this;
		}
		explicit operator bool() const {return !content.empty();}
		Stack& operator<<(const T& val) {
			push(val);
			return *this;
		}
		Stack& operator<<(T&& val) {
			push(std::move(val));
			return *this;
		}
		template<typename C>
		Stack& operator<<(const C& arr) {
			typedef typename std::conditional<
				is_iterable<C>::value && !std::is_same<C, T>::value,
				StackIterablePusher<C>,
				StackElementPusher<C>>::type pusher_type;
			pusher_type().push(*this, arr);
			return *this;
		}
		Stack& operator--() {
			pop();
			return *this;
		}
		Stack& operator--(int) {
			// Same as ++(*this);
			pop();
			return *this;
		}
		T& operator*() {return content.front();}
		const T& operator*() const {return content.front();}
	};
}

#endif // CIGMAR_STACK
