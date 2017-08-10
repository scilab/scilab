// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI Group - Clement David
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

loadXcosLibs();

// check input value
for s=["FATAL", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE"]
    scicos_log(s);
end

// check output value
assert_checkequal(scicos_log("TRACE"), "TRACE");


// model objects

// allocate a Block
o = scicos_block();
assert_checkequal(scicos_log("refCounters"), [1 0 0]);
clear o;
assert_checkequal(scicos_log("refCounters"), []);

// allocate a Diagram
scs_m = scicos_diagram();
assert_checkequal(scicos_log("refCounters"), [2 1 0]);
clear scs_m;
assert_checkequal(scicos_log("refCounters"), []);

// allocate a Link
o = scicos_link();
assert_checkequal(scicos_log("refCounters"), [3 2 0]);
clear o;
assert_checkequal(scicos_log("refCounters"), []);

// allocate an Annotation
o = TEXT_f("define");
assert_checkequal(scicos_log("refCounters"), [4 3 0]);
clear o;
assert_checkequal(scicos_log("refCounters"), []);


