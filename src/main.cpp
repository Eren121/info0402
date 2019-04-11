#ifndef OWN_MAIN
#	define OWN_MAIN 0
#endif

#if OWN_MAIN
#	define CATCH_CONFIG_RUNNER
#else
#	define CATCH_CONFIG_MAIN
#endif

#include "catch.hpp"
