// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//

//
// nominal check using a user-defined function
//

function foo()
    2
endfunction
profileEnable(foo)

// Executes the function
foo();

prof = profileGetInfo();
assert_checkequal(prof.FunctionTable.FunctionName, "foo");
assert_checkequal(size(prof.LineCoverage), 1);
assert_checkequal(prof.LineCoverage(1)(:,1), [-1;1;-1]);

//
// check with inner functions
//

function with_inner()
    2
    function inner()
        4
    endfunction
    6
endfunction
profileEnable(with_inner)

// execute
with_inner()

prof = profileGetInfo();
assert_checkequal(prof.FunctionTable.FunctionName, ["foo" ; "with_inner" ; "inner"]);

//
// check API using Scilab functions
//

profileEnable(iscolumn) // from elementary_functionslib
// check that foo, with_inner, inner and iscolumn are instrumented
prof = profileGetInfo();
assert_checkequal(prof.FunctionTable.FunctionName, ["foo" ; "with_inner" ; "inner" ; "iscolumn"]);

profileEnable(corelib)
// check that at least publicly visible function are instrumented (inner functions are not visible)
assert_checktrue(size(profileGetInfo().LineCoverage) > 4 + size(libraryinfo("corelib"), "*"));

profileEnable()
// check that more than corelib and elementary_functionslib are instrumented
assert_checktrue(size(profileGetInfo().LineCoverage) > 4 + size(libraryinfo("corelib"), "*") + size(libraryinfo("elementary_functionslib"), "*"));

profileDisable()
