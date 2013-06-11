// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// =============================================================================
// tan
// =============================================================================

function demo_tan()

    exec("SCI/modules/graphics/demos/cmplxfunc/MacCmplx.sci", -1);
    R     = %pi/2-0.15;
    theta = -130;
    alpha = 73;

    my_handle = scf(100001);
    clf(my_handle,"reset");

    PlotCmplxFunc(R,0,"Square","Ox",41,"tan",theta,alpha,[-R,R]);
    demo_viewCode("demo_tan.dem.sce");

endfunction

demo_tan()
clear demo_tan;
