// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- Non-regression test for bug 7761 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7716
//
// <-- Short Description -->
// Block inter-functions were not loaded after using the xcosPal API and then
// starting Xcos.

exec(SCI + "/modules/scicos/macros/scicos_scicos/standard_define.sci", -1);
exec(SCI + "/modules/scicos/macros/scicos_scicos/scicos_model.sci", -1);
exec(SCI + "/modules/scicos/macros/scicos_scicos/scicos_graphics.sci", -1);
exec(SCI + "/modules/scicos/macros/scicos_scicos/scicos_block.sci", -1);

// loading only the SUM_f block
exec(SCI + "/modules/scicos_blocks/macros/Linear/SUM_f.sci", -1);

scs_m = SUM_f("define");
save(TMPDIR + "/sum.sod", "scs_m");
pal = xcosPal("SamplePal");
blockstyle = struct();
pal = xcosPalAddBlock(pal, TMPDIR + "/sum.sod", SCI + "/modules/xcos/images/palettes/SUM_f.png", blockstyle);
xcosPalExport(pal, TMPDIR + "/mypal.sod");

clear all; // emulate a scilab restart
xcosPalAdd(TMPDIR + "/mypal.sod");

xcos();

if ~isdef("BIGSOM_f") then pause, end

