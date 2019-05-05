#ifndef DEFINES_H
#define DEFINES_H

#include <utility>
#include <ostream>

#define OWN_MAIN	0
#define DEBUG		1

#if DEBUG
	#include <exception>
	class DebugException : public virtual std::logic_error {
	public:
		using logic_error::logic_error;
	};

	#define ASSERT_LOCATION(file, line) " Line " #line ", File " file
	#define ASSERT(x) do { if(!(x)) throw DebugException(#x ASSERT_LOCATION(__FILE__, __LINE__)); } while(0)
#else
	#define ASSERT(x)
#endif

namespace pair_operators {

	template<typename U, typename V>
	std::ostream& operator<<(std::ostream& lhs, const std::pair<U, V>& pair) {

		lhs << "{" << pair.first << "," << pair.second << "}";
		return lhs;
	}
}


#endif // DEFINES_H
