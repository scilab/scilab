// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// =============================================================================
// acosh
// =============================================================================

function demo_acosh()

    exec("SCI/modules/graphics/demos/cmplxfunc/MacCmplx.sci", -1);
    R     = 2;
    e     = %eps;
    theta = -110;
    alpha = 75;

    my_handle = scf(100001);
    clf(my_handle,"reset");

    PlotCmplxFunc(R,e,"Square","Ox",41,"acosh",theta,alpha,[1,R]);
    demo_viewCode("demo_acosh.dem.sce");

endfunction

demo_acosh()
clear demo_acosh;
