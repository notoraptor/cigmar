#include <iostream>
#include "utils.hpp"
#include "unittests.hpp"
using std::cout;
using std::endl;

utt_begin(Tests);

utt(test_string) {
	String s = "Bonjour, comment vas-tu ?";
	String t;
	utt_assert(s.length() == 25);
	utt_assert(s[0] == 'B');
	utt_assert(s[LAST] == '?');
	utt_assert(s[s.length() - 1] == s[LAST]);
	utt_assert(s[9] == 'c');
	utt_assert(s);
	utt_assert(!t);
	s.clear();
	utt_assert(s == t);
	s = "Salamandar";
	t = "tyrannosaurus";
	s.swap(t);
	utt_assert(s == "tyrannosaurus" && t == "Salamandar");
	s << " rex";
	s.insert(0, "The ");
	s[4] = 'T';
	utt_assert(s == "The Tyrannosaurus rex");
	s.replace("Tyrannosaurus", "Parasaurolophus").insert(8, "lititan and the Para") << '.';
	utt_assert("The Paralititan and the Parasaurolophus rex." == s);
	String r = "carcharodontosaurus";
	pos_t p1 = s.indexOf("Paralititan");
	pos_t p2 = s.lastIndexOf("rex");
	pos_t p3 = s.indexOf("calamitisaura");
	utt_assert((bool)p1);
	utt_assert((bool)p2);
	utt_assert(!p3);
	s.replace((size_t)p1, (size_t)(p2 - p1 - 1), r);
	utt_assert(s == "The carcharodontosaurus rex.");
	ArrayList<String> p = s.split(" ");
	ArrayList<String> q = s.split("-");
	ArrayList<String> n = s.split(" ");
	utt_assert(p.size() == 3);
	utt_assert(n.size() == 3);
	utt_assert(q.size() == 1);
	utt_assert(q[0] == (s << ""));
	utt_assert(q[0] == (s << ""));
	s = "my name is Steven, I am 27, and I live in Montreal, Canada.";
	s.extractTo(t, 11, 6);
	utt_assert("Steven" == t);
	utt_assert(s.substring(19, 7) == "I am 27");
	ArrayList<String> spaced = s.split(" ");
	ArrayList<String> commas = s.split(",");
	utt_assert(spaced.size() == 13);
	utt_assert(commas.size() == 4);
	s = "Bonjour";
	utt_assert(s.toLower() == "bonjour");
	utt_assert(s.toUpper() == "BONJOUR");
	utt_assert(("abc"_s << 2 << true << -1.5 << ' ' << false) == "abc2true-1.5 false");
	utt_assert("abc2true-1.5 false" == String("abc", 2, true, -1.5, ' ', false));
	s[LAST] = 'l';
	utt_assert(s == "Bonjoul");
	s = "Bonjour, comment vas-tu ?";
	t = "Bonjour";
	utt_assert(s.startsWith("Bonjour"));
	utt_assert(s.startsWith(t));
	utt_assert(s[0] == 'B');
	t = "tu ?";
	utt_assert(s.endsWith("tu ?"));
	utt_assert(s.endsWith(t));
	utt_assert(s[LAST] == '?');
	size_t count = countof(s);
	utt_assert(count == s.length());
	const String cs(s);
	count = countof(cs);
	utt_assert(count == cs.length());
	char buffer[7];
	utt_assert(s.memout(buffer, 9, 7) == 7);
	utt_assert(buffer[0] == 'c');
	utt_assert(buffer[6] == 't');
	utt_assert(s.extractTo(buffer, 9, 6) == 7);
	utt_assert(String(buffer) == "commen");
}

utt(test_pos_t) {
	String s = "Bonjour";
	pos_t p1 = s.indexOf("soir");
	pos_t p2 = s.lastIndexOf('o');
	pos_t p3 = s.indexOf('o');
	pos_t p4 = s.indexOf('t');
	utt_assert(!p1);
	utt_assert(!p4);
	utt_assert(p2);
	utt_assert(p3);
	utt_assert(p1 == p4);
	utt_assert(p2 != p3);
	utt_assert(p2 != p4);
	utt_assert(p2 > p3);
	utt_assert(p3 < p2);
	utt_assert(p2 == 4);
	utt_assert(1 == p3);
	p1 *= 2;
	++p4;
	utt_assert(!p1);
	utt_assert(!p4);
	--p2;
	utt_assert(p2);
	utt_assert(p2 == 3);
	++p3;
	p3 *= 3;
	utt_assert(p3);
	utt_assert(6 == p3);
	p3 -= 6;
	utt_assert(p3);
	utt_assert(p3 == 0);
	--p3;
	utt_assert(!p3);
	++p3;
	utt_assert(!p3);
	p3 = 12;
	utt_assert(p3);
	utt_assert(p3 == 12);
	p3 = 0;
	utt_assert(p3);
	utt_assert(0 == p3);

	p3 += p2;
	p3 -= 1;
	p3 += p2;
	utt_assert(p3);
	utt_assert(p3 == 5);
	p3 = p1;
	utt_assert(!p3);
	p3 = p2;
	utt_assert(p3 == 3);
	pos_t x = p1 + 6*p2 - p3 -2*p4;
	utt_assert(!x);
	x = p2 * p3 + 4*p2 -5*p3 + (p2 + p3) % p2;
	utt_assert(x);
	x = p2 - p3;
	utt_assert(x);
	x %= 0;
	utt_assert(!x);
	x = p2 - 2*p3;
	utt_assert(!x);
	x = (4*p3 - 1) / p2;
	utt_assert(x);
	utt_assert(3 == x);
}

utt(test_return_t) {
	int x = 2;
	auto a1 = from_stack<int>(x);
	auto a2 = a1;
	auto a3 = a2;
	auto a4 = a3;
	a2 = a4;
	utt_assert(!check(a1));
	utt_assert(check(a2));
	utt_assert(!check(a3));
	utt_assert(!check(a4));
}

utt(test_array_t) {
	array_t<long long int, 8> r;
	numbers::zeros(r);
	utt_assert(numbers::sum(r) == 0);
	numbers::ones(r);
	utt_assert(numbers::sum(r) == r.size());
	numbers::add(r, 12);
	numbers::add(r, -1);
	numbers::mul(r, -3.5);
	utt_assert(r[0] == r[LAST]);
	utt_assert(r[r.size() / 2] == -42);
	numbers::random::uniform(r, 5, 34);
	long long int x = numbers::sum(r);
	long long int y = -37;
	numbers::sum(r, y, false);
	utt_assert(y == x - 37);
	numbers::sum(r, y);
	utt_assert(x == y);
	array_t<double, 44> u(0);
	array_t<double, 44> v(-1);
	utt_assert(numbers::sum(u) == 0);
	utt_assert(numbers::sum(v) == -(double)v.size());
}

utt(test_ArrayList) {
	ArrayList<int> r(8);
	numbers::zeros(r);
	utt_assert(numbers::sum(r) == 0);
	numbers::ones(r);
	utt_assert(numbers::sum(r) == (int)r.size());
	numbers::add(r, 12);
	numbers::add(r, -1);
	numbers::mul(r, -3.5);
	utt_assert(r[0] == r[LAST]);
	utt_assert(r[r.size() / 2] == -42);
	numbers::random::uniform(r, 5, 34);
	long long int x = numbers::sum(r);
	long long int y = -37;
	numbers::sum(r, y, false);
	utt_assert(y == x - 37);
	numbers::sum(r, y);
	utt_assert(x == y);
	ArrayList<double> u(44, 0);
	ArrayList<double> v(44, -1);
	utt_assert(numbers::sum(u) == 0);
	utt_assert(numbers::sum(v) == -(double)v.size());
}

utt(test_TreeSet) {
	array_t<double, 50> arr;
	array_t<int, 600> arr2;
	array_t<long, 100> arr3;
	numbers::random::binomial(arr, 50, 0.6);
	numbers::random::uniform(arr2, 1, 500);
	numbers::random::normal(arr3, 100, 25);
	utt_assert(numbers::all(arr, [](double x) {return x == (size_t)x && x >= 0 && x < 50;}));
	utt_assert(numbers::all(arr2, [](int x) {return x >= 1 && x <= 500;}));
	TreeSet<double> s(arr), s2(arr2), s3(arr3);
	utt_assert(s.size() < arr.size());
	utt_assert(s2.size() < arr2.size());
	utt_assert(s3.size() < arr3.size());
	utt_assert(s);
	utt_assert(s2);
	utt_assert(s3);
	utt_assert(!TreeSet<int>());
	TreeSet<size_t> s4;
	s4 << 1 << 2 << 5 << 33 << 5 << 27 << 2 << 1 << 1;
	utt_assert(s4 && s4.size() == 5);
	utt_assert(s4.contains(33));
	utt_assert(!s4.contains(29));
	s4.remove(33).remove(27);
	utt_assert(!s4.contains(33));
	utt_assert(!s4.contains(27));
	utt_assert(s4.contains(2));
	double min = s.min();
	double max = s.max();
	double before = s.before((min + max)/2);
	double after = s.after((min + max)/2);
	utt_assert(min < max);
	utt_assert(before < (min + max)/2);
	utt_assert(after > (min + max)/2);
	utt_assert(min <= before);
	utt_assert(min <= after);
	utt_assert(max >= before);
	utt_assert(max >= after);
	double previous = min;
	for (double x : s) {
		utt_assert(previous <= x);
		previous = x;
	}
	TreeSet<double> reversed_set(s, [](double a, double b) {return a > b;});
	previous = max;
	for (double x: reversed_set) {
		utt_assert(previous >= x);
		previous = x;
	}
	const double* ptr_max = s.safeMax();
	const double* ptr_min = s.safeMin();
	utt_assert(ptr_max != nullptr);
	utt_assert(ptr_min);
	utt_assert(max == *ptr_max);
	utt_assert(min == *ptr_min);
	utt_assert(s.safeBefore(-1) == nullptr);
	utt_assert(!s.safeAfter(50));
	utt_assert(s.safeAfter(-1));
	utt_assert(s.safeBefore(50));
}

utt_end();

int main() {
	numbers::rng.seed(1788599792);
	Tests()();
	return 0;
}
