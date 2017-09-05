// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
// Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
// Copyright (C) 2017 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- TEST WITH XCOS -->
// <-- INTERACTIVE TEST -->

// we launch xcos then libs should be loaded
xcos();

// we launch xcos with an xcos demo file
xcos(SCI + "/modules/xcos/demos/Simple_Demo.zcos");

// we launch xcos with an xcos demo file with full path resolution
xcos("SCI/modules/xcos/demos/Simple_Demo.zcos");

// we launch xcos with cosf file
xcos(SCI + "/modules/scicos/palettes/Branching.cosf");

// we launch xcos with a scs_m instance
scs_m = scicos_diagram();
scs_m.objs($+1) = BIGSOM_f("define");
xcos(scs_m);

// we launch xcos with a full scs_m
importXcosDiagram(SCI+"/modules/xcos/demos/Discrete-KalmanFilter.zcos");
xcos(scs_m);

// we launch xcos with a scs_m subsystem
assert_checkequal(typeof(scs_m.objs(1).model.rpar), "diagram");
xcos(scs_m.objs(1).model.rpar);

