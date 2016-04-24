// fast_set.h

/***
Let T be an unsigned integral type other than bool.  Then fast_set<T>(N) can represent a set of 
elements of type T, in the range [0,N), or [0,N-1] where N < T::max_value.  We do this
using two vectors of size N: one is an elements list, the list of the elements currently 
present in the list.  The other is a list of positions of the elements in the elements list,
such that: positions[e] == T::max_value indicates that element e is NOT in the set, and 
positions[e] == X indicates that element e is in position X of the elements list.  

Note: to keep memory usage under control, T should be the smallest unsigned integer type that can hold N.

Let S = fast_set<T>(N), where N is the capacity of the set, and n = |S|, and e \in S.  This container provides:

O(1) insert(e), remove(e), and contains(e)
O(n) enumerations (no order guarantees)
O(N) ordered enumerations (guaranteed *it < *(it+1))
O(1) access to unordered list of elements, consequently O(1) uniform selection
O(2N) words of storage
------------------------
O(N) union, intersection and difference (not currently implemented).
***/


#ifndef _FAST_SET_INCLUDED_
#define _FAST_SET_INCLUDED_

#include <vector>
#include <iterator>
#include <stdexcept>
#include <cassert>

class iterator_not_dereferenceable_exception : public std::runtime_error
{
	public:
		iterator_not_dereferenceable_exception() :
			std::runtime_error("iterator not dereferenceable!")
		{
		}
};

template <typename UnsignedIntType>
class fast_set {

	static_assert(std::is_unsigned<UnsignedIntType>::value, "Template parameter must be unsigned.");
	static_assert(!std::is_same<UnsignedIntType, bool>::value, "Template parameter must not be bool.");

	public:
		typedef typename std::vector<UnsignedIntType>::const_iterator const_iterator;
		typedef UnsignedIntType element_type;

		const_iterator cbegin() const
		{
			return _elements.cbegin();
		}
		
		const_iterator cend() const
		{
			return _elements.cbegin() + _num_elements;
		}

		fast_set() : 
			_capacity(UnsignedIntType(-1)-1),
			_num_elements(0),
			_elements(_capacity),
			_positions(_capacity, UnsignedIntType(-1))
		{
			assert(_capacity < UnsignedIntType(-1));
		}

		fast_set(UnsignedIntType capacity) :
			_capacity(capacity),
			_num_elements(0),
			_elements(_capacity),
			_positions(_capacity, UnsignedIntType(-1))
		{
			assert(_capacity < UnsignedIntType(-1));
		}

		~fast_set()
		{
		}

		bool insert(UnsignedIntType element)
		{
			assert(element >= 0 && element < _capacity);
			assert(_num_elements >= 0 && _num_elements <= _capacity);

			if (_positions[element] != NO_VALUE) {
				return false;
			}
			_positions[element] = _num_elements;
			_elements[_num_elements++] = element;
			return true;
		}

		bool remove(UnsignedIntType element)
		{
			assert(element >= 0 && element < _capacity);
			assert(_num_elements >= 0 && _num_elements <= _capacity);

			UnsignedIntType remove_pos = _positions[element];	// where is the element to remove?
			if (remove_pos == NO_VALUE) {
				return false;
			}
			// Use the old trick: overwrite the removed element with the last_element, decrement the element count,
			// update the position of the removed element to NO_VALUE, and finally set the position of the last_element
			// to the position of the removed element.  There is a subtle bug if the removed element ==  last_element,
			// so we always want to set _positions[element] = NO_VALUE last to avoid the decision element != last_element.
			UnsignedIntType last_element = _elements[--_num_elements];		// what is the last_element?
			_positions[last_element] = remove_pos;					// set last_element's position to where the removed element was
			_elements[remove_pos] = last_element;					// place the last element where removed element was
			_positions[element] = NO_VALUE;							// remove the element
			return true;
		}

		bool contains(UnsignedIntType element) const
		{
			assert(element >= 0 && element < _capacity);
			assert(_num_elements >= 0 && _num_elements <= _capacity);

			return _positions[element] != NO_VALUE;
		}

		bool is_empty() const
		{
			assert(_num_elements >= 0 && _num_elements <= _capacity);

			return _num_elements == 0;
		}

		uint64_t size() const
		{
			assert(_num_elements >= 0 && _num_elements <= _capacity);

			return _num_elements;
		}

		UnsignedIntType operator[](UnsignedIntType i)
		{
			assert(i >= 0 && i < _capacity);
			assert(_num_elements >= 0 && _num_elements <= _capacity);

			return _elements[i];
		}

		const UnsignedIntType& operator[](UnsignedIntType i) const
		{
			assert(i >= 0 && i < _capacity);
			assert(_num_elements >= 0 && _num_elements <= _capacity);

			return _elements[i];
		}

		UnsignedIntType uniform_select(double p)
		{
			assert(p >= 0.0 && p < 1.0);
			assert(_num_elements >= 0 && _num_elements <= _capacity);

			UnsignedIntType index = (UnsignedIntType)(p * _num_elements);
			return _elements[index];
		}

		uint64_t capacity() const
		{
			return _capacity;
		}

		void show() const
		{
			std::cout << "size: " << uint64_t(_num_elements) << std::endl;
			std::cout << "elements : ";
			for (UnsignedIntType i = 0; i < _num_elements; ++i) {
				std::cout << uint64_t(_elements[i]);
				if (i < _num_elements - 1) {
					std::cout << ' ';
				}
			}
			std::cout << std::endl << "positions: ";
			for (UnsignedIntType i = 0; i < _capacity; ++i) {
				if (_positions[i] == NO_VALUE) {
					std::cout << '-';
				} else {
					std::cout << uint64_t(_positions[i]);
				}
				if (i < _capacity - 1) {
					std::cout << ' ';
				}
			}
			std::cout << std::endl;
		}

	private:
		uint64_t 						_capacity;
		uint64_t 						_num_elements;
		std::vector<UnsignedIntType>	_elements;
		std::vector<UnsignedIntType>	_positions;

		const UnsignedIntType NO_VALUE = -1;
		
		
	public:
		#ifdef ordered_iterator_impl
		class const_ordered_iterator : public std::iterator<
			std::bidirectional_iterator_tag,
			UnsignedIntType>
		{
			public:
				// default ctor: uninitialized iterator, nothing should work except to assign to this object
				const_ordered_iterator() : ref_set(nullptr), pos(0), first(0)
				{
				}

				// copy ctor
				const_ordered_iterator(const const_ordered_iterator& ref) : ref_set(ref.ref_set), pos(ref.pos), first(ref.first)
				{
				}
			
				// constructor guaranteed to point to first dereferenceable element (ie: begin()),  or end()
				const_ordered_iterator(const fast_set<UnsignedIntType>* set, bool endFlag = false) :
					ref_set(set), pos(0), first(0)
				{
					// find the first dereferenceable position, and mark it as first
					while (pos < ref_set->capacity() && ref_set->_positions[pos] == UnsignedIntType(-1)) {
						++pos;
					}
					first = pos;
					if (endFlag) {	// flag the end() iterator appropriately
						pos = ref_set->capacity();
					}
				}
				
				const_ordered_iterator& operator=(const const_ordered_iterator& other)
				{
					if (this != &other) {
						ref_set = other.ref_set;
						pos = other.pos;
						first = other.first;
					}
				}

				~const_ordered_iterator()
				{
				}

				const_ordered_iterator operator++()
				{
					if (ref_set != nullptr) {
						while (++pos < ref_set->capacity() && ref_set->_positions[pos] == UnsignedIntType(-1));
					}
					return *this;
				}

				const_ordered_iterator operator++(int)
				{
					const_ordered_iterator tmp(*this); operator++(); return tmp;
				}

				const_ordered_iterator operator+(int delta) const
				{
					const_ordered_iterator it(*this);
					while (delta--) {
						++it;
					}
					return it;
				}
				
				const_ordered_iterator operator-(int delta) const
				{
					const_ordered_iterator it(*this);
					while (delta--) {
						--it;
					}
					return it;
				}

				const_ordered_iterator operator--()
				{
					if (ref_set != nullptr && pos > first) {
						while (--pos > first && ref_set->_positions[pos] == UnsignedIntType(-1));
					}
					return *this;
				}

				const_ordered_iterator operator--(int)
				{
					const_ordered_iterator tmp(this); operator--(); return tmp;
				}

				UnsignedIntType operator*() const
				{
					if (ref_set == nullptr) {	// if there is no container, there is nothing we can do but throw
						throw iterator_not_dereferenceable_exception();
					}
					return pos;
				}

			private:
				const fast_set<UnsignedIntType>* ref_set;
				UnsignedIntType pos;
				UnsignedIntType first;

			friend bool operator==(const fast_set<UnsignedIntType>::const_ordered_iterator& a, const fast_set<UnsignedIntType>::const_ordered_iterator& b)
			{
				return a.ref_set == b.ref_set && a.pos == b.pos;
			}

			friend bool operator!=(const fast_set<UnsignedIntType>::const_ordered_iterator& a, const fast_set<UnsignedIntType>::const_ordered_iterator& b)
			{
				return !(a == b);
			}

			friend bool operator<(const fast_set<UnsignedIntType>::const_ordered_iterator& a, const fast_set<UnsignedIntType>::const_ordered_iterator& b)
			{
				return a.ref_set == b.ref_set && (a.pos < b.pos);
			}

			// implementation: n + it
			friend const_ordered_iterator operator+(int delta, const const_ordered_iterator& other)
			{
				return other + delta;
			}

			// caveat: no implementation of n - it;
			// friend const_ordered_iterator operator+(int delta, const const_ordered_iterator& other);
		};
		
		const_ordered_iterator ordered_cbegin() const
		{
			return const_ordered_iterator(this);
		}

		const_ordered_iterator ordered_cend() const
		{
			return const_ordered_iterator(this, true);
		}

		#endif //ordered_iterator_impl

};

/*	================================================
	////////////////////////////////////////////////
	// idiom for reverse iteration of const_iterator
	////////////////////////////////////////////////
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
	
	/////////////////////////////////////////////////////////
	// idiom for reverse iteration of const_ordered_iterator
	/////////////////////////////////////////////////////////
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
	================================================
*/

#endif //_FAST_SET_INCLUDED_

