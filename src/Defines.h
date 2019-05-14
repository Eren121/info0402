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


#	define STR(x) #x
#	define ASSERT_LOCATION(file, line) " Line " STR(line) ", File " STR(file)
#	define ASSERT(x) do { if(!(x)) throw DebugException(#x ASSERT_LOCATION(__FILE__, __LINE__)); } while(0)
#	define ASSERT_THROW(message) throw DebugException(message ASSERT_LOCATION(__FILE__, __LINE__))
#else
#	define ASSERT(x)
#	define ASSERT_THROW(x)
#endif

namespace pair_operators {

	template<typename U, typename V>
	std::ostream& operator<<(std::ostream& lhs, const std::pair<U, V>& pair) {

		lhs << "{" << pair.first << "," << pair.second << "}";
		return lhs;
	}
}

template<typename T>
std::ostream& operator<<(std::ostream& lhs, const std::initializer_list<T>& initl) {

	auto e = initl.end();
	auto it = initl.begin();

	lhs << "{";

	while(it != e) {

		lhs << *it;

		++it;

		if(it != e) {
			lhs << ',';
		}
	}

	lhs << "}";

	return lhs;
}

#endif // DEFINES_H
