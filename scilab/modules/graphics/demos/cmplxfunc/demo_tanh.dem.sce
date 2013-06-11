// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// =============================================================================
// tanh
// =============================================================================

function demo_tanh()

    exec("SCI/modules/graphics/demos/cmplxfunc/MacCmplx.sci", -1);

    R     = %pi/2-0.2;
    e     = 0;
    theta = -130;
    alpha = 73;

    my_handle = scf(100001);
    clf(my_handle,"reset");

    PlotCmplxFunc(R,e,"Square","Ox",41,"tanh",theta,alpha,[-R,R]);
    demo_viewCode("demo_tanh.dem.sce");

endfunction

demo_tanh();
clear demo_tanh;
