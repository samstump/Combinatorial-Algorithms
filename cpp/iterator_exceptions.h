// iterator_exceptions.h

#include <stdexcept>

class iterator_not_dereferenceable_exception : public std::runtime_error
{
	public:
		iterator_not_dereferenceable_exception() :
			std::runtime_error("iterator not dereferenceable!")
		{
		}
};