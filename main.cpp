#include <iostream>
#include "utils.hpp"
using std::cout;
using std::endl;

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

utt_end();

int main() {
	TestCigmar()();
	return 0;
}
