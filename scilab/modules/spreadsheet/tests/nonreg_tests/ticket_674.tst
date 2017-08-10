// =============================================================================
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
// <-- Non-regression test for bug 674 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/674/
//
// <-- Short Description -->
// csvDefault returns %f when used twice
// =============================================================================
assert_checkequal(csvDefault('eol', 'linux'), %t)
assert_checkequal(csvDefault('eol', 'linux'), %t)
assert_checkequal(csvDefault('eol', 'freestyle'), %f)
// =============================================================================


