// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// =============================================================================
// asin
// =============================================================================

function demo_asin()

    exec("SCI/modules/graphics/demos/cmplxfunc/MacCmplx.sci", -1);
    theta = -110;
    alpha = 75;

    my_handle = scf(100001);
    clf(my_handle,"reset");

    PlotCmplxFunc(2,%eps,"Square","Ox",41,"asin",theta,alpha,[-1,1]);
    demo_viewCode("demo_asin.dem.sce");

endfunction

demo_asin()
clear demo_asin;
