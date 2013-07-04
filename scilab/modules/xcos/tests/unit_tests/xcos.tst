// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
// Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- TEST WITH GRAPHIC -->

// libs are not loaded at startup
if isdef('BIGSOM_f') then pause, end

// we launch xcos then libs should be loaded
xcos();
sleep(500);
if ~isdef('BIGSOM_f') then pause, end

// we launch xcos with an xcos demo file
xcos(SCI + "/modules/xcos/demos/Simple_Demo.zcos");
sleep(500);

// we launch xcos with an xcos demo file with full path resolution
xcos("SCI/modules/xcos/demos/Simple_Demo.zcos");
sleep(500);

// we launch xcos with cos file
xcos(SCI + "/modules/xcos/tests/nonreg_tests/Antrieb3.cos");
sleep(500);

// we launch xcos with cosf file
xcos(SCI + "/modules/scicos/palettes/Branching.cosf");
sleep(500);

// we launch xcos with a scs_m instance
scs_m = scicos_diagram();
scs_m.objs($+1) = BIGSOM_f("define");
xcos(scs_m);
sleep(500);

