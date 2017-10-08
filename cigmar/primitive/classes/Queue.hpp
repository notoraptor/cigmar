#ifndef CIGMAR_QUEUE
#define CIGMAR_QUEUE

#include <forward_list>
#include <initializer_list>
#include <cigmar/primitive/utils.hpp>

namespace cigmar {

template<typename T>
class Queue {
private:
	template<typename E>
	struct QueueElementPusher {
		void push(Queue& queue, const E& element) {
			queue.push(element);
		};
	};
	template<typename E>
	struct QueueIterablePusher {
		void push(Queue& queue, const E& iterable) {
			queue.pushAll(iterable);
		};
	};
public:
	typedef T dtype;
	typedef std::forward_list<T> queue_type;
	typedef typename std::forward_list<T>::iterator iterator_t;
	typedef typename std::forward_list<T>::const_iterator const_iterator_t;
private:
	queue_type content;
	const_iterator_t last_it;
	size_t length;
public:
	Queue(): content(), last_it(content.before_begin()), length(0) {}
	template<typename C>
	Queue(const C& arr): Queue() {pushAll(arr);}
	Queue(std::initializer_list<T> il): Queue() {pushAll(il);}
	Queue(const Queue& other): Queue() {pushAll(other.content);}
	Queue(Queue&&) = default;

	size_t size() const {return length;}
	Queue& push(const T& val) {
		last_it = content.insert_after(last_it, val);
		++length;
		return *this;
	}
	Queue& push(T&& val) {
		last_it = content.insert_after(last_it, std::move(val));
		++length;
		return *this;
	}
	template<typename C>
	Queue& pushAll(const C& arr) {
		last_it = content.insert_after(last_it, arr.begin(), arr.end());
		length += arr.size();
		return *this;
	}
	Queue& pop() {
		content.pop_front();
		if (content.empty()) last_it = content.before_begin();
		--length;
		return *this;
	}
	Queue& swap(Queue& other) {
		queue_type temp_content(content);
		content.clear();
		last_it = content.insert_after(content.before_begin(), other.content.begin(), other.content.end());
		other.content.clear();
		other.last_it = other.content.insert_after(other.content.before_begin(), temp_content.begin(), temp_content.end());
		std::swap(length, other.length);
		return *this;
	}
	Queue& clear() {
		content.clear();
		last_it = content.before_begin();
		length = 0;
		return *this;
	}
	T& front() {return content.front();}
	const T& front() const {return content.front();}

	iterator_t begin() {return content.begin();}
	iterator_t end() {return content.end();}
	const_iterator_t begin() const {return content.begin();}
	const_iterator_t end() const {return content.end();}

	Queue& operator=(const Queue& other) {
		clear();
		pushAll(other.content);
		return *this;
	}
	Queue& operator=(Queue&&) = default;
	Queue& operator=(std::initializer_list<T> il) {
		clear();
		pushAll(il);
		return *this;
	}
	template<typename C>
	Queue& operator=(const C& arr) {
		static_assert(is_iterable<C>::value, "Only an iterable can be affected to a queue.");
		clear();
		pushAll(arr);
		return *this;
	}
	explicit operator bool() const {return !content.empty();}
	Queue& operator<<(const T& val) {
		push(val);
		return *this;
	}
	Queue& operator<<(T&& val) {
		push(std::move(val));
		return *this;
	}
	template<typename C>
	Queue& operator<<(const C& arr) {
		typedef typename std::conditional<
			is_iterable<C>::value && !std::is_same<C, T>::value,
			QueueIterablePusher<C>,
			QueueElementPusher<C>>::type pusher_type;
		pusher_type().push(*this, arr);
		return *this;
	}
	Queue& operator++() {
		pop();
		return *this;
	}
	Queue& operator++(int) {
		// Same as ++(*this);
		pop();
		return *this;
	}
	T& operator*() {return content.front();}
	const T& operator*() const {return content.front();}
};

}

#endif // CIGMAR_QUEUE
