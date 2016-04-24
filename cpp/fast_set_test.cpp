#include <iostream>
#include <random>

#define ordered_iterator_impl
#include "fast_set.h"

#include "superkiss64.h"


void show2(const fast_set<uint8_t>& set)
{
	std::cout << "set = {";
	for (auto it = set.cbegin(); it != set.cend(); ++it) {
		std::cout << (uint64_t)*it;
		if (it + 1 < set.cend()) std::cout << ' ';
	}
	std::cout << "}" << std::endl;

	std::cout << "ordered set = {";
	for (auto it = set.ordered_cbegin(); it != set.ordered_cend(); ++it) {
		std::cout << (uint64_t)*it;
		if (it + 1 < set.ordered_cend()) std::cout << ' ';
	}
	std::cout << "}" << std::endl;
	
	fast_set<uint8_t>::const_ordered_iterator x;
//	++x;

	if (x != set.ordered_cend()) {
		std::cout << "not end?" << std::endl;
		try {
			std::cout << (uint64_t)*x << std::endl;
		} catch (const iterator_not_dereferenceable_exception& ex) {
			std::cout << "exception: " << ex.what() << std::endl;
		}
	}

	x = set.ordered_cbegin();
	for (int c = 0; c < 5; ++c) {
		x++;
	}
	std::cout << "*it = " << (uint64_t)*x << std::endl;
	std::cout << "*(it-1) = " << (uint64_t)*(x-1) << std::endl;
	std::cout << "*(1+it) = " << (uint64_t)*(1+x) << std::endl;


	std::cout << "reverse set = {";
	auto it2 = set.cend() - 1;
	if (it2 != set.cend()) {
		for (;;) {
			std::cout << (uint64_t)*it2;
			if (it2 == set.cbegin()) {
				break;
			}
			std::cout << ' ';
			--it2;
		}
	}
	std::cout << "}" << std::endl;
	
	std::cout << "reverse ordered set = {";
	auto it = set.ordered_cend() - 1;
	if (it != set.ordered_cend()) {
		for (;;) {
			std::cout << (uint64_t)*it;
			if (it == set.ordered_cbegin()) {
				break;
			}
			std::cout << ' ';
			--it;
		}
	}
	std::cout << "}" << std::endl;
}

int main()
{
	std::random_device rd;
	superkiss64 rng(rd(), rd(), rd());
	
	fast_set<uint8_t> set(50);
	
	set.insert(45);
	set.insert(17);
	set.insert(5);
	set.insert(10);
	set.insert(0);
	set.insert(21);
	set.insert(12);
	
	show2(set);
	
	set.insert(25);
	show2(set);


}