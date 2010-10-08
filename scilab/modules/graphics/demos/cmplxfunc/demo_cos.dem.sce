// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// =============================================================================
// cos
// =============================================================================

function demo_cos()

    exec("SCI/modules/graphics/demos/cmplxfunc/MacCmplx.sci", -1);
    R     = %pi;
    theta = 18;
    alpha = 43;

    my_handle = scf(100001);
    clf(my_handle,"reset");

    PlotCmplxFunc(R,0,"Disk","Ox",[40 20],"cos",theta,alpha,[-R,R]);
    demo_viewCode("demo_cos.dem.sce");

endfunction

demo_cos()
clear demo_cos;
