// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// =============================================================================
// atan
// =============================================================================

function demo_atan()

    exec("SCI/modules/graphics/demos/cmplxfunc/MacCmplx.sci", -1);
    R     = 2;
    theta = -110;
    alpha = 75;

    my_handle = scf(100001);
    clf(my_handle,"reset");

    PlotCmplxFunc(R,0.001,"Square","Oy",41,"atan",theta,alpha,[-R,R]);
    demo_viewCode("demo_atan.dem.sce");

endfunction

demo_atan()
clear demo_atan;
