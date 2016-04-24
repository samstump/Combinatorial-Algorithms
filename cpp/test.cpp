#include <iostream>

#include "powerset.h"

std::ostream& operator<<(std::ostream& o, const std::vector<bool>& p)
{
    o << '{';
    for (auto it = p.begin(); it != p.end(); ++it) {
        o << *it;
        if (it + 1 < p.end()) o << ", ";
    }
    return o << '}';
}

std::ostream& operator<<(std::ostream& o, const std::vector<int>& p)
{
    o << '{';
    for (auto it = p.begin(); it != p.end(); ++it) {
        o << *it;
        if (it + 1 < p.end()) o << ", ";
    }
    return o << '}';
}

void enumerate_powerset(powerset& ps)
{
    int count = 0;
    for (auto it = ps.begin(); it != ps.end(); ++it) {
        std::cout << ++count << ": " << (*it).get_bitvec()  << "  |  " << (*it).get_listvec()  << std::endl;
    }
}

int main()
{
    try {
        powerset_graycode g(5);
        enumerate_powerset(g);
        std::cout << std::endl;

        powerset_binary b(5);
        enumerate_powerset(b);
        std::cout << std::endl;

        powerset_lexicographic l5(5);
        enumerate_powerset(l5);
        std::cout << std::endl;    

        powerset_lexicographic l(5, {1,2}, true);
        enumerate_powerset(l);

    } catch (const iterator_not_dereferenceable_exception& ex) {
        std::cout << "exception: " << ex.what() << std::endl;
    }

}
