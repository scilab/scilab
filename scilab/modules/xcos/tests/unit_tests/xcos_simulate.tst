// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->


// Load diagram as mlist
load(SCI+"/modules/xcos/tests/unit_tests/SimpleGENSINExport.sod");
assert_checkequal(type(scs_m), 17);


[%cpr, ok] = xcos_simulate(scs_m, 4);

assert_checktrue(ok);
assert_checkequal(data.time, (0:0.1:29.9)');
assert_checkequal(data.values, sin(data.time));

clear scs_m
load(SCI+"/modules/xcos/tests/unit_tests/SimpleGENSINSCOPE.sod");
assert_checkequal(type(scs_m), 17);

scs_m.props.tf = 30;
[%cpr, ok] = xcos_simulate(scs_m, 4);
assert_checktrue(ok);


// Load diagram as userType
clear scs_m
load(SCI+"/modules/xcos/tests/unit_tests/SimpleGENSINExport2.sod");
assert_checkequal(type(scs_m), 128);

scs_m.props.tf = 30;
[%cpr, ok] = xcos_simulate(scs_m, 4);

assert_checktrue(ok);
assert_checkequal(data.time, (0:0.1:29.9)');
assert_checkequal(data.values, sin(data.time));

clear scs_m
load(SCI+"/modules/xcos/tests/unit_tests/SimpleGENSINSCOPE2.sod");
assert_checkequal(type(scs_m), 128);

scs_m.props.tf = 30;
[%cpr, ok] = xcos_simulate(scs_m, 4);
assert_checktrue(ok);
