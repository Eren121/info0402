#include "Defines.h"

///
/// Fichier qui défini le main de catch dans le cas de USE_CUSTOM_MAIN == 0
///

#if USE_CUSTOM_MAIN
    #define CATCH_CONFIG_RUNNER
#else
    #define CATCH_CONFIG_MAIN
#endif

#include "catch.hpp"
