/*
 *  Chocobo1/Hash
 *
 *   Copyright 2017-2018 by Mike Tzou (Chocobo1)
 *     https://github.com/Chocobo1/Hash
 *
 *   Licensed under GNU General Public License 3 or later.
 *
 *  @license GPL3 <https://www.gnu.org/licenses/gpl-3.0-standalone.html>
 */

#define CATCH_CONFIG_RUNNER
#include "catch2/single_include/catch2/catch.hpp"


int main(int argc, char *argv[])
{
	// utilize ccache:
	// since this file is always the same and it's compilation is slow
	// so put the tests in other translation units to speed up compilation

	return Catch::Session().run(argc, argv);
}
