// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- XCOS TEST -->

// Load diagram as mlist
load(SCI+"/modules/xcos/tests/unit_tests/SimpleGENSINExport.sod");
assert_checkequal(type(scs_m), 17);

[%cpr, ok] = xcos_simulate(scs_m, 4);

assert_checktrue(ok);
assert_checkequal(data.time, (0:0.1:29.9)');
assert_checkequal(data.values, sin(data.time));

load(SCI+"/modules/xcos/tests/unit_tests/SimpleGENSINSCOPE.sod");
assert_checkequal(type(scs_m), 17);

[%cpr, ok] = xcos_simulate(scs_m, 4);
assert_checkfalse(ok); // Cannot use scope in without graphics


// Load diagram as userType
load(SCI+"/modules/xcos/tests/unit_tests/SimpleGENSINExport2.sod");
assert_checkequal(type(scs_m), 128);

[%cpr, ok] = xcos_simulate(scs_m, 4);

assert_checktrue(ok);
assert_checkequal(data.time, (0:0.1:29.9)');
assert_checkequal(data.values, sin(data.time));

load(SCI+"/modules/xcos/tests/unit_tests/SimpleGENSINSCOPE2.sod");
assert_checkequal(type(scs_m), 128);

[%cpr, ok] = xcos_simulate(scs_m, 4);
assert_checkfalse(ok); // Cannot use scope without graphics
