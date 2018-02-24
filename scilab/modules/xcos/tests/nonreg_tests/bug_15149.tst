// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI Group - Clement DAVID
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 15149 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15149
//
// <-- Short Description -->
// Some schema file were corrupted on save and reload
//

loadXcosLibs();

// load and save
scs_m1 = xcosDiagramToScilab("SCI/modules/xcos/tests/nonreg_tests/bug_15149.zcos");
xcosDiagramToScilab("TMPDIR/foo.zcos", scs_m1);

// reload #1
scs_m2 = xcosDiagramToScilab("TMPDIR/foo.zcos");
// reload #2
scs_m3 = xcosDiagramToScilab("TMPDIR/foo.zcos");

// check that objs have the same size
assert_checkequal(size(scs_m1.objs), size(scs_m2.objs));
assert_checkequal(size(scs_m1.objs), size(scs_m3.objs));

// check that all blocks are connected
for i=1:size(scs_m1.objs)
    o = scs_m1.objs(i);
    if typeof(o) == "Block" then
        assert_checktrue(o.graphics.pin <> 0);
        assert_checktrue(o.graphics.pout <> 0);
        assert_checktrue(o.graphics.pein <> 0);
        assert_checktrue(o.graphics.peout <> 0);
    end
end
for i=1:size(scs_m2.objs)
    o = scs_m2.objs(i);
    if typeof(o) == "Block" then
        assert_checktrue(o.graphics.pin <> 0);
        assert_checktrue(o.graphics.pout <> 0);
        assert_checktrue(o.graphics.pein <> 0);
        assert_checktrue(o.graphics.peout <> 0);
    end
end
for i=1:size(scs_m3.objs)
    o = scs_m3.objs(i);
    if typeof(o) == "Block" then
        assert_checktrue(o.graphics.pin <> 0);
        assert_checktrue(o.graphics.pout <> 0);
        assert_checktrue(o.graphics.pein <> 0);
        assert_checktrue(o.graphics.peout <> 0);
    end
end


// simulate scs_m1 and scs_m2
// warnings are possible but scs_m2 simulation should pass and should not
// destroy scs_m1 nor scs_m3 objects

scicos_simulate(scs_m1, "nw");

// check that objs have the same size
assert_checkequal(size(scs_m1.objs), size(scs_m2.objs));
assert_checkequal(size(scs_m1.objs), size(scs_m3.objs));

// check that all blocks are connected
for i=1:size(scs_m1.objs)
    o = scs_m1.objs(i);
    if typeof(o) == "Block" then
        assert_checktrue(o.graphics.pin <> 0);
        assert_checktrue(o.graphics.pout <> 0);
        assert_checktrue(o.graphics.pein <> 0);
        assert_checktrue(o.graphics.peout <> 0);
    end
end
for i=1:size(scs_m2.objs)
    o = scs_m2.objs(i);
    if typeof(o) == "Block" then
        assert_checktrue(o.graphics.pin <> 0);
        assert_checktrue(o.graphics.pout <> 0);
        assert_checktrue(o.graphics.pein <> 0);
        assert_checktrue(o.graphics.peout <> 0);
    end
end
for i=1:size(scs_m3.objs)
    o = scs_m3.objs(i);
    if typeof(o) == "Block" then
        assert_checktrue(o.graphics.pin <> 0);
        assert_checktrue(o.graphics.pout <> 0);
        assert_checktrue(o.graphics.pein <> 0);
        assert_checktrue(o.graphics.peout <> 0);
    end
end


scicos_simulate(scs_m2, "nw");

// check that objs have the same size
assert_checkequal(size(scs_m1.objs), size(scs_m2.objs));
assert_checkequal(size(scs_m1.objs), size(scs_m3.objs));

// check that all blocks are connected
for i=1:size(scs_m1.objs)
    o = scs_m1.objs(i);
    if typeof(o) == "Block" then
        assert_checktrue(o.graphics.pin <> 0);
        assert_checktrue(o.graphics.pout <> 0);
        assert_checktrue(o.graphics.pein <> 0);
        assert_checktrue(o.graphics.peout <> 0);
    end
end
for i=1:size(scs_m2.objs)
    o = scs_m2.objs(i);
    if typeof(o) == "Block" then
        assert_checktrue(o.graphics.pin <> 0);
        assert_checktrue(o.graphics.pout <> 0);
        assert_checktrue(o.graphics.pein <> 0);
        assert_checktrue(o.graphics.peout <> 0);
    end
end
for i=1:size(scs_m3.objs)
    o = scs_m3.objs(i);
    if typeof(o) == "Block" then
        assert_checktrue(o.graphics.pin <> 0);
        assert_checktrue(o.graphics.pout <> 0);
        assert_checktrue(o.graphics.pein <> 0);
        assert_checktrue(o.graphics.peout <> 0);
    end
end

clear scs_m1
clear scs_m2
clear scs_m3

