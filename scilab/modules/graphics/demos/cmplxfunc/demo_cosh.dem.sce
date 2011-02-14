// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// =============================================================================
// cosh
// =============================================================================

function demo_cosh()

    exec("SCI/modules/graphics/demos/cmplxfunc/MacCmplx.sci", -1);
    R     = %pi;
    e     = 0;
    theta = -130;
    alpha = 56;

    my_handle = scf(100001);
    clf(my_handle,"reset");

    PlotCmplxFunc(R,e,"Disk","Ox",[40 20],"cosh",theta,alpha,[-R,R]);
    demo_viewCode("demo_cosh.dem.sce");
endfunction

demo_cosh()
clear demo_cosh;
