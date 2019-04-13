#include "multimap/multimap.h"

void testTemplateInstanciation() {
	multimap<int, int> mymap;
	multimap_iterator<int, int> it(mymap);
}


