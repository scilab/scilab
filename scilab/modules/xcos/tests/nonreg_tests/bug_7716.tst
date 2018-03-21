// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
// <-- Non-regression test for bug 7761 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7716
//
// <-- Short Description -->
// Block inter-functions were not loaded after using the xcosPal API and then
// starting Xcos.

scs_m = SUM_f("define");
save("sum.sod", "scs_m");
pal = xcosPal("SamplePal");
blockstyle = struct();
pal = xcosPalAddBlock(pal, "sum.sod", SCI + "/modules/xcos/images/palettes/SUM_f.png", blockstyle);
xcosPalExport(pal, "mypal.sod");

// restart Scilab
xcosPalAdd("mypal.sod");

xcos();

if ~isdef("BIGSOM_f") then pause, end

