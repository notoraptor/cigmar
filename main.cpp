#include <iostream>
#include "utils.hpp"
using std::cout;
using std::endl;

#define dtypeof(variable) decltype(variable)::dtype

utt_begin(TestCigmar);

utt(test_sets) {
	TreeSet<int> ts = {1, 2, 1, 1, 7, -1, 5, -1, 1};
	HashSet<int> hs = {1, 2, 1, 1, 7, -1, 5, -1, 1};
	utt_assert(ts && hs);
	utt_assert(ts.size() == hs.size());
	for (int i: ts) utt_assert(hs.contains(i));
	for (const int& i: hs) utt_assert(ts.contains(i));
}

utt(test_TreeMap) {
	TreeMap<String, int> m;
	m["Bonjour"] = 1;
	m["Bonsoir"] = 2;
	m["Bonne nuit"] = 3;
	utt_assert(m["Bonsoir"] == 2);
	utt_assert(m.size() == 3);
	m["Bonsoir"] = 5;
	m["Bonne nuit"] = -7;
	utt_assert(m.size() == 3);
	utt_assert(m["Bonsoir"] == 5);
	utt_assert(-7 == m["Bonne nuit"]);
	utt_assert(1 == m["Bonjour"]);
	utt_assert(5 == m["Bonsoir"]);
	utt_assert(m.contains("Bonjour"));
	utt_assert(!m.contains("Bontour"));
	m.remove("Bonjour");
	utt_assert(m.size() == 2);
	m.remove("salut");
	utt_assert(m.size() == 2);
    m.clear();
    utt_assert(!m);
    TreeMap<String, int> m2 = { {"a", 1}, {"b", 2}, {"salut", 7} };
    m.swap(m2);
    utt_assert(m.size() == 3);
    utt_assert(m.get("salut") == 7);
    m.put("a", 100);
    utt_assert(m.size() == 3);
    utt_assert(m["a"] == 1);
    m = { {"A", 1}, {"B", 2}, {"LAST", 27}, {"Z", 26} };
    utt_assert(m.size() == 4);
    utt_assert(m["LAST"] == 27);
}

utt(test_HashMap) {
	HashMap<String, int> m;
	m["Bonjour"] = 1;
	m["Bonsoir"] = 2;
	m["Bonne nuit"] = 3;
	utt_assert(m["Bonsoir"] == 2);
	utt_assert(m.size() == 3);
	m["Bonsoir"] = 5;
	m["Bonne nuit"] = -7;
	utt_assert(m.size() == 3);
	utt_assert(m["Bonsoir"] == 5);
	utt_assert(-7 == m["Bonne nuit"]);
	utt_assert(1 == m["Bonjour"]);
	utt_assert(5 == m["Bonsoir"]);
	utt_assert(m.contains("Bonjour"));
	utt_assert(!m.contains("Bontour"));
	m.remove("Bonjour");
	utt_assert(m.size() == 2);
	m.remove("salut");
	utt_assert(m.size() == 2);
    m.clear();
    utt_assert(!m);
    HashMap<String, int> m2 = { {"a", 1}, {"b", 2}, {"salut", 7} };
    m.swap(m2);
    utt_assert(m.size() == 3);
    utt_assert(m.get("salut") == 7);
    m.put("a", 100);
    utt_assert(m.size() == 3);
    utt_assert(m["a"] == 1);
    m = { {"A", 1}, {"B", 2}, {"LAST", 27}, {"Z", 26} };
    utt_assert(m.size() == 4);
    utt_assert(m["LAST"] == 27);
}

utt(test_Stack) {
	Stack<int> s1;
	utt_assert(!s1);
	utt_assert(s1.size() == 0);
	ArrayList<double> arr = {1, 2, 3, 4};
	Stack<int> s2(arr);
	utt_assert(s2);
	utt_assert(s2.size() == 4);
	utt_assert(*s2 == 4);
	Stack<double> s3 = {1.7, -2.9, 3.36};
	utt_assert(s3);
	utt_assert(s3.size() == 3);
	utt_assert(*s3 == 3.36);
	--s3;
	utt_assert(s3);
	utt_assert(s3.size() == 2);
	utt_assert(*s3 == -2.9);
	--s3;
	utt_assert(s3);
	utt_assert(s3.size() == 1);
	utt_assert(1.7 == *s3);
	--s3;
	utt_assert(!s3);
	utt_assert(s3.size() == 0);
	Stack<size_t> stack;
	stack.push(12).push(27).push(61);
	utt_assert(stack.size() == 3);
	utt_assert(*stack == 61);
	stack.pop();
	utt_assert(stack.size() == 2);
	utt_assert(*stack == 27);
	stack.push(11).push(15).pop().pop().pop();
	utt_assert(*stack == 12);
	utt_assert(stack.front() == 12);
	Stack<size_t> other = {10, 11, 12, 13, 14, 15};
	stack.swap(other);
	utt_assert(stack.size() == 6);
	utt_assert(other.size() == 1);
	utt_assert(*other == 12);
	utt_assert(*stack == 15);
	utt_assert(*stack.begin() == 15);
	stack = arr;
	utt_assert(stack.size() == 4);
	utt_assert(*stack == 4);
	stack = {7, 8, 11, 24, 29};
	utt_assert(stack.size() == 5);
	utt_assert(*stack == 29);
	for (int i = 0; i < 5; ++i) --stack;
	utt_assert(!stack);
	utt_assert(stack.size() == 0);

	stack << 100 << 200 << 300 << 1111 << 400;
	utt_assert(stack.size() == 5);
	utt_assert(*stack == 400);
	--stack;
	utt_assert(*stack == 1111);
	--stack;
	utt_assert(*stack == 300);
	utt_assert(stack.size() == 3);
	stack << arr;
	utt_assert(stack.size() == 3 + 4);
	utt_assert(*stack == 4);
	stack.clear();
	utt_assert(!stack);
	Stack<std::vector<long>> vs;
	std::vector<long> l = {1, -2, 3}, l1 = {4, -5, 6}, l2 = {7, 8, 9, 10};
	vs << l;
	utt_assert(vs.size() == 1);
	vs << l1 << l2;
	utt_assert(vs.size() == 3);
	utt_assert((*vs)[3] == 10);
	vs.pop();
	--vs;
	utt_assert(vs.size() == 1);
	utt_assert((*vs)[1] == -2);
	vs << (std::vector<long>){1, 2, 3, 4};
	vs << (dtypeof(vs)){-1, -2, 3, -4};
	for (auto& v:vs) cout << ArrayList<long>(v) << endl;
}

utt(test_Queue) {
	Queue<int> s1;
	utt_assert(!s1);
	utt_assert(s1.size() == 0);
	ArrayList<double> arr = {1, 2, 3, 4};
	Queue<int> s2(arr);
	utt_assert(s2);
	utt_assert(s2.size() == 4);
	utt_assert(*s2 == 1);
	Queue<double> s3 = {1.7, -2.9, 3.36};
	utt_assert(s3);
	utt_assert(s3.size() == 3);
	utt_assert(*s3 == 1.7);
	++s3;
	utt_assert(s3);
	utt_assert(s3.size() == 2);
	utt_assert(*s3 == -2.9);
	++s3;
	utt_assert(s3);
	utt_assert(s3.size() == 1);
	utt_assert(3.36 == *s3);
	++s3;
	utt_assert(!s3);
	utt_assert(s3.size() == 0);
	Queue<size_t> queue;
	queue.push(12).push(27).push(61);
	utt_assert(queue.size() == 3);
	utt_assert(*queue == 12);
	queue.pop();
	utt_assert(queue.size() == 2);
	utt_assert(*queue == 27);
	queue.push(11).push(15).pop().pop().pop();
	utt_assert(*queue == 15);
	utt_assert(queue.front() == 15);
	Queue<size_t> other = {10, 11, 12, 13, 14, 15};
	queue.swap(other);
	utt_assert(queue.size() == 6);
	utt_assert(other.size() == 1);
	utt_assert(*other == 15);
	utt_assert(*queue == 10);
	utt_assert(*queue.begin() == 10);
	queue = arr;
	utt_assert(queue.size() == 4);
	utt_assert(*queue == 1);
	queue = {7, 8, 11, 24, 29};
	utt_assert(queue.size() == 5);
	utt_assert(*queue == 7);
	for (int i = 0; i < 5; ++i) ++queue;
	utt_assert(!queue);
	utt_assert(queue.size() == 0);

	queue << 100 << 200 << 300 << 1111 << 400;
	utt_assert(queue.size() == 5);
	utt_assert(*queue == 100);
	++queue;
	utt_assert(*queue == 200);
	++queue;
	utt_assert(*queue == 300);
	utt_assert(queue.size() == 3);
	queue << arr;
	utt_assert(queue.size() == 3 + 4);
	utt_assert(*queue == 300);
	queue.clear();
	utt_assert(!queue);
	Queue<std::vector<long>> vs;
	std::vector<long> l = {1, -2, 3}, l1 = {4, -5, 6}, l2 = {7, 8, 9, 10};
	vs << l;
	utt_assert(vs.size() == 1);
	vs << l1 << l2;
	utt_assert(vs.size() == 3);
	utt_assert((*vs)[1] == -2);
	vs.pop();
	++vs;
	utt_assert(vs.size() == 1);
	utt_assert((*vs)[1] == 8);
	vs << (std::vector<long>){1, 2, 3, 4};
	vs << (dtypeof(vs)){-1, -2, 3, -4};
	for (auto& v:vs) cout << ArrayList<long>(v) << endl;
}

utt_end();

int main() {
	TestCigmar()();
	return 0;
}
