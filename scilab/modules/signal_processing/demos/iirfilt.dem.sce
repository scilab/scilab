// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_irrfilter()

    ////////////////////////
    //DISCRETE IIR FILTERS//
    ////////////////////////

    //Low pass filter (length 5, type cheb1, cut-off .2)
    hz = iir(5, "lp", "cheb1", [.2 0], [.05 .05]);
    [hzm,fr] = frmag(hz,256);

    my_handle = scf(100001);
    if is_handle_valid(my_handle) then
        clf(my_handle, "reset");
        plot2d(fr', hzm');
        xtitle(_("Discrete IIR filter: low pass  fr < 0.2"), " ", " ");
        halt(_("Press Return to continue ... \n"));
    end

    //Band pass filter (length 3, type ellip, cut-offs .15 and .25)
    hz = iir(3, "bp", "ellip", [.15 .25], [.08 .03]);
    [hzm,fr] = frmag(hz,256);
    if is_handle_valid(my_handle) then
        clf(my_handle, "reset");
        demo_viewCode("iirfilt.dem.sce");
        plot2d(fr',hzm');
        xtitle(_("Discrete IIR filter: band pass 0.15 < fr < 0.25"), " ", " ");
    end

endfunction

demo_irrfilter();
clear demo_irrfilter;
