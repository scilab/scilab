// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// =============================================================================
// exp
// =============================================================================

function demo_exp()

    exec("SCI/modules/graphics/demos/cmplxfunc/MacCmplx.sci", -1);

    R     = 2;
    theta = -130;
    alpha = 73;

    my_handle = scf(100001);
    clf(my_handle,"reset");

    PlotCmplxFunc(R,0,"Disk","Ox",[40 20],"exp",theta,alpha,[-R,R]);
    demo_viewCode("demo_exp.dem.sce");

endfunction

demo_exp()
clear demo_exp;
