#define OWN_MAIN 0

#if OWN_MAIN
	#define CATCH_CONFIG_RUNNER
#else
	#define CATCH_CONFIG_MAIN
#endif

#include <catch/catch.hpp>

#if OWN_MAIN
	#include <iostream>

	using namespace std;

	int main(int argc, char* argv[]) {

		cout << "ok" << endl;
		return 0;
	}
#endif
