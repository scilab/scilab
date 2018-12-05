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

// helpers

function simple()
    2
endfunction

function with_inner()
    2
    function inner()
        4
    endfunction
    6
endfunction

// API check
profileEnable(simple)
profileDisable(simple)
assert_checkequal(size(profileGetInfo().LineCoverage), 0);
assert_checkequal(size(profileGetInfo().FunctionCoverage), [0 0]);
assert_checkequal(size(profileGetInfo().FunctionTable), [0 0]);

profileEnable(with_inner)
profileDisable(with_inner)
assert_checkequal(size(profileGetInfo().LineCoverage), 0);
assert_checkequal(size(profileGetInfo().FunctionCoverage), [0 0]);
assert_checkequal(size(profileGetInfo().FunctionTable), [0 0]);

profileEnable(iscolumn) // from elementary_functionslib
profileDisable(iscolumn)
assert_checkequal(size(profileGetInfo().LineCoverage), 0);
assert_checkequal(size(profileGetInfo().FunctionCoverage), [0 0]);
assert_checkequal(size(profileGetInfo().FunctionTable), [0 0]);

profileEnable(whos) // from corelib with sub-macros
profileDisable(whos)
assert_checkequal(size(profileGetInfo().LineCoverage), 0);
assert_checkequal(size(profileGetInfo().FunctionCoverage), [0 0]);
assert_checkequal(size(profileGetInfo().FunctionTable), [0 0]);

profileEnable(corelib)
profileDisable(corelib)
assert_checkequal(size(profileGetInfo().LineCoverage), 0);
assert_checkequal(size(profileGetInfo().FunctionCoverage), [0 0]);
assert_checkequal(size(profileGetInfo().FunctionTable), [0 0]);

profileEnable()
profileDisable()
assert_checkerror("profileGetInfo", "profileGetInfo: profile is disabled.");
