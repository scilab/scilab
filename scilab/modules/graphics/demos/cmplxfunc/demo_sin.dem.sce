// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// =============================================================================
// sin
// =============================================================================

function demo_sin()

    exec("SCI/modules/graphics/demos/cmplxfunc/MacCmplx.sci", -1);

    R     = %pi;
    theta = -130;
    alpha = 73;
    e     = 0.001;

    my_handle = scf(100001);
    clf(my_handle,"reset");

    PlotCmplxFunc(R,e,"Disk","Ox",[40 20],"sin",theta,alpha,[-R,R]);
    demo_viewCode("demo_sin.dem.sce");

endfunction

demo_sin()
clear demo_sin;
