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
	size_t count = 0;
	for (char c:s) ++count;
	utt_assert(count == s.length());
	const String cs(s);
	count = 0;
	for (const char& c:cs) ++count;
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
	array_t<size_t, 14> r;
}

utt_end();

int main() {
	Tests()();
	return 0;
}
