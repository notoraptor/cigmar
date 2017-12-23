//
// Created by notoraptor on 17-12-21.
//
#include <cigmar/classes/String.hpp>
#include <cigmar/unittests.hpp>

using namespace cigmar;
utt_begin(TestString);
	utt(test_empty_constructor) {
		String s;
		utt_assert(sizeof(String::char_t) == 1);
		utt_assert(s.isEmpty());
		utt_assert(!s);
		utt_assert(s.length() == 0);
	}
	utt(test_basic_constructor) {
		String s = "Hello!";
		utt_assert(s);
		utt_assert(!s.isEmpty());
		utt_assert(s.length() == 6);
	}
	utt(test_with_char_types) {
		String s1 = "Bonjour";
		String s2 = L"Bonjour";
		String s3 = u8"Bonjour";
		String s4 = u"Bonjour";
		String s5 = U"Bonjour";
		String s6 = R"(Bonjour)";
		utt_assert(s1.length() == 7);
		utt_assert(s2.length() == 7);
		utt_assert(s3.length() == 7);
		utt_assert(s4.length() == 7);
		utt_assert(s5.length() == 7);
		utt_assert(s6.length() == 7);
	}
	utt(test_substring_constructor_from_carray) {
		const char* ls = "Hello, world!";
		String s1(ls, 0);
		String s2(ls, 0, 5);
		String s3(ls, 7);
		String s4(ls, 7, 3);
		utt_assert(s1 == ls);
		utt_assert(ls == s1);
		utt_assert(!(ls != s1));
		utt_assert(!(s1 != ls));
		utt_assert(s2 == "Hello");
		utt_assert("world!" == s3);
		utt_assert(s4 == "wor");
		utt_assert(s4 != "woR");
		utt_assert("Wor" != s4);
	}
	utt(test_substring_constructor_from_other) {
		String ls = "Hello, world!";
		String s1(ls, 0);
		String s2(ls, 0, 5);
		String s3(ls, 7);
		String s4(ls, 7, 3);
		utt_assert(s1 == ls);
		utt_assert(!(s1 != ls));
		utt_assert(s2 == "Hello");
		utt_assert("world!" == s3);
		utt_assert(s4 == "wor");
	}
	utt(test_concatenations_and_splits) {
		utt_assert(!String::write());
		utt_assert(!String::print());
		utt_assert(String::writeln());
		utt_assert(String::println());
		utt_assert(String::writeln() == ENDL);
		utt_assert(String::println() == ENDL);
		std::string a = "ab";
		int b = 2;
		bool c = false;
		String d = U"Hello";
		char e = '?';
		uint64_t f = 100000000000000000;
		const char16_t* g = u"[1] 部首 国字 木, 神!";
		const char32_t* h = U"[2] 部首 国字 木, 神!";
		const char* test_string_written = u8"ab2falseHello?100000000000000000[1] 部首 国字 木, 神![2] 部首 国字 木, 神!";
		const char* test_string_printed = u8"ab 2 false Hello ? 100000000000000000 [1] 部首 国字 木, 神! [2] 部首 国字 木, 神!";
		String s1 = String::write(a, b, c, d, e, f, g, h);
		String s2 = String::print(a, b, c, d, e, f, g, h);
		utt_smart_assert(s1 == test_string_written, s1);
		utt_smart_assert(s2 == test_string_printed, s2);
		ArrayList<const char*> pieces = {"a", "bc", "d", "ef", "g"};
		String s3 = String::join(pieces);
		String s4 = String::join(pieces, 'a');
		String s5 = String::join(pieces, "-__-");
		utt_smart_assert(s3 == "abcdefg", s3);
		utt_smart_assert(s4 == "aabcadaefag", s4);
		utt_smart_assert(s5 == "a-__-bc-__-d-__-ef-__-g", s5);
		std::string t = "abc";
		ArrayList<String> e1 = s3.split("");
		ArrayList<String> e2 = s4.split("a");
		ArrayList<String> e3 = s5.split("-__-");
		utt_smart_assert(e1.size() == 0, e1.size());
		utt_smart_assert(e2.size() == 6, e2.size());
		utt_smart_assert(e3.size() == 5, e3.size());
	}
	utt(test_splits) {
		String s1 = "Hello, how are you?";
		ArrayList<String> p1 = s1.split(' ');
		ArrayList<String> p2 = s1.split(" ");
		ArrayList<String> p3 = s1.split("o");
		utt_smart_assert(p1.size() == 4, p1.size());
		utt_smart_assert(p2.size() == 4, p2.size());
		utt_smart_assert(p3.size() == 4, p3.size());
	}
	utt(test_affectation) {
		// u8"zß水𝄋" <=> "z\u00df\u6c34\U0001d10b";
		const char* ref = u8"zß水𝄋";
		String s = "Hello";
		utt_assert(s == "Hello");
		s = L"zß水𝄋";
		utt_assert(s == ref);
		s = u"zß水𝄋";
		utt_assert(s == ref);
		s = U"zß水𝄋";
		utt_assert(s == ref);
		s = "Ablabla";
		utt_assert(s == "Ablabla");
	}
	utt(test_append) {
		String s = "Hello";
		s << 1;
		s << ' ' << 2.5 << true;
		s << U"zß水𝄋" << "    " << 'c';
		utt_smart_assert(s == "Hello1 2.5truezß水𝄋    c", s);
	}
	utt(test_modifications_and_extractions) {
		String s = "Hello";
		utt_assert(s == "Hello");
		s.clear();
		utt_assert(!s);
		String t = "Super";
		s = "Hello";
		s.swap(t);
		utt_assert(s == "Super");
		utt_assert(t == "Hello");
		s.insert(0, U"Hyper");
		utt_smart_assert(s == "HyperSuper", s);
		s.insert(5, "Mega");
		utt_smart_assert(s == "HyperMegaSuper", s);
		s.replace(5, 3, u"Ultr");
		utt_smart_assert(s == "HyperUltraSuper", s);
		s.replace("per", "mar");
		utt_smart_assert(s == "HymarUltraSumar", s);
		s.remove(2, 9);
		utt_smart_assert(s == "Hyumar", s);
		t.replace('l', 't');
		utt_smart_assert(t == "Hetto", t);
		utt_assert(s.leftTrimmable() == 0);
		utt_assert(s.rightTrimmable() == 0);
		utt_assert(s.leftTrimmable("Hyu") == 3);
		utt_assert(s.rightTrimmable("ar") == 2);
		String u = "    \t    fef4u4r r4ij3 43j4 44\t\t\t  \t \f\v\t  ";
		utt_assert(u.leftTrimmable() == 9);
		utt_assert(u.rightTrimmable() == 12);
		utt_assert(u().trimLeft() == "fef4u4r r4ij3 43j4 44\t\t\t  \t \f\v\t  ");
		utt_assert(u().trimRight() == "    \t    fef4u4r r4ij3 43j4 44");
		utt_assert(u.trim() == "fef4u4r r4ij3 43j4 44");
		utt_assert(u.trim("4j ") == "fef4u4r r4ij3 43");
		utt_assert(u().upper() == "FEF4U4R R4IJ3 43");
		utt_assert(u.upper().lower() == "fef4u4r r4ij3 43");
		char buffer[6];
		u.substringInto(buffer, 8, 5);
		utt_assert(buffer[0] == 'r');
		utt_assert(buffer[1] == '4');
		utt_assert(buffer[2] == 'i');
		utt_assert(buffer[3] == 'j');
		utt_assert(buffer[4] == '3');
		utt_assert(buffer[5] == '\0');
		u.charactersInto(buffer, 1, 6);
		utt_assert(buffer[0] == 'e');
		utt_assert(buffer[1] == 'f');
		utt_assert(buffer[2] == '4');
		utt_assert(buffer[3] == 'u');
		utt_assert(buffer[4] == '4');
		utt_assert(buffer[5] == 'r');
	}
	utt(test_content) {
		String s = "fef4u4r r4ij3 43";
		utt_assert(s.startsWith("fef4u4r "));
		utt_assert(s.endsWith("j3 43"));
		utt_assert(!s.endsWith("fef4u4r "));
		utt_assert(!s.startsWith("j3 43"));
		utt_assert(s.contains('r'));
		utt_assert(!s.contains('w'));
		utt_assert(s.contains('r', 7));
		utt_assert(!s.contains('f', 7));
		utt_assert(s.contains("r4ij3"));
		utt_assert(s.contains("43", 13));
		utt_assert(!s.contains("43", 15));
		utt_assert(!s.contains("r4ij4"));
		utt_assert(s.indexOf('4'));
		utt_assert(s.indexOf('4') == 3);
		utt_assert(s.lastIndexOf('4') == 14);
		utt_assert(s.indexOf('4', 6) == 9);
		utt_assert(s.indexOf("r4ij3") == 8);
	}
	utt(test_comparisons) {
		String a1 = "assert";
		String a2 = "assert";
		String b = "b";
		String c = "c";
		utt_assert(a1 == a1);
		utt_assert(!(a1 != a1));
		utt_assert(a1 == a2);
		utt_assert(!(a1 != a2));
		utt_assert(b != c);
		utt_assert(!(b == c));
		utt_assert(b < c);
		utt_assert(!(b > c));
		utt_assert(!(b >= c));
		utt_assert(b > a1);
		utt_assert(!(b < a1));
		utt_assert(!(b <= a1));
		utt_assert(a1 <= a2);
		utt_assert(!(a1 > a2));
		utt_assert(a1 <= b);
		utt_assert(!(a1 > b));
		utt_assert(a2 >= a1);
		utt_assert(!(a2 < a1));
		utt_assert(c >= a1);
		utt_assert(b != "c");
		utt_assert(b <= "b");
		utt_assert(b <= "c");
		utt_assert(b < "c");
		utt_assert(b > "a");
		utt_assert(b >= "b");
		utt_assert(b >= "a");
	}
utt_end();
