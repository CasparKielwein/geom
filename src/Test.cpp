#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include "geom.h"

struct tag {
	int t = 0;

	bool operator==(tag o) const {
		return t == o.t;
	}
};
using tagged_vec3d = geom::object<3,tag>;

void geom_initialize() {

	geom::collection<tagged_vec3d> empty_col { };

	ASSERT(empty_col.empty());
	ASSERT(empty_col.size() == 0);
	ASSERT(empty_col.begin() == empty_col.end());
	ASSERT(empty_col.cbegin() == empty_col.cend());

	geom::collection<tagged_vec3d> two_elements {
		tagged_vec3d { { 0, 0, 0 }, tag { 0 } },
		tagged_vec3d { { 1, 1, 1 }, tag { 0 } } };

	ASSERT(!two_elements.empty());
	ASSERT(two_elements.size() == 2);
	ASSERT(two_elements.begin() != two_elements.end());
	ASSERT(two_elements.cbegin() != two_elements.cend());

}

void geom_loop() {

	geom::collection<tagged_vec3d> col {
		tagged_vec3d { { 0, 0, 0 }, tag { 0 } },
		tagged_vec3d { { 1, 1, 1 }, tag { 0 } } };

	tagged_vec3d test_val { { 0, 0, 0 }, tag { 0 } };

	for (auto x : col) {
		ASSERT(x == test_val);
	}

	for (tagged_vec3d x : col) {
		ASSERT(x == test_val);
	}

	for (auto&& x : col) {
		ASSERT(x == test_val);
	}


	ASSERT(col.size() == 2);

}

bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };

	s.push_back(CUTE(geom_initialize));
	s.push_back(CUTE(geom_loop));

	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner { cute::makeRunner(lis, argc, argv) };
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
	return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
