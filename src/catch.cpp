#include "Defines.h"

#if OWN_MAIN
	#define CATCH_CONFIG_RUNNER
#else
	#define CATCH_CONFIG_MAIN
#endif

#include <catch/catch.hpp>

template<typename T>
struct A {

	void f() {
		T t = 3.3f;
	}

	T t = 3.3f;
};

template<typename T>
struct is_true {
	enum {
		value = true
	};
};

template<typename T>
struct B {

	static_assert(is_true<decltype(&A<T>>::f)>::value, "");
};

void f() {
	B<std::string> b;
}
