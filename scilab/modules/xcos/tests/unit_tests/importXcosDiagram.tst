// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2012 - Clément DAVID
//
// This file is distributed under the same license as the Scilab package.

// <-- XCOS TEST -->
//
// <-- Short Description -->
// Check importXcosDiagram on some diagrams


error_count = 0;
try
    importXcosDiagram("not_existing_file.zcos");
catch
    error_count = error_count + 1;
end
assert_checkequal(error_count, 1);

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/demos/Simple_Demo.zcos"));

assert_checkequal(exists('scs_m'), 1);
assert_checkequal(typeof(scs_m), "diagram");

assert_checktrue(length(scs_m.objs) > 0);

previous = scs_m;
clear scs_m

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/demos/Simple_Demo.zcos"));
assert_checktrue(isequal(scs_m, previous));
