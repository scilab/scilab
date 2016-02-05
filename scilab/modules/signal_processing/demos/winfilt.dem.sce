// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_winfilt()

    ////////////////////////
    //WINDOWED FIR FILTERS//
    ////////////////////////

    // Low pass filter (length 33, Kaiser window, cut-off .2)
    [wft, wfm, fr] = wfir("lp",33,[.2 0],"kr",[5.6 0]);

    my_handle = scf(100001);
    if is_handle_valid(my_handle) then
        clf(my_handle,"reset");
        demo_viewCode("winfilt.dem.sce");
        plot2d(fr', log(wfm)')
        xtitle(" ", _("frequency"), _("magnitude"));
        xtitle([_("Windowed FIR Low pass Filter"); ..
        _("Kaiser window, cut-off: 0.2"); ..
        _("length 33")]);

        halt(_("Press Return to continue ... \n"));

    end

    // Stop band filter (length 127, Hamming window, cut-offs .2 and .3)
    [wft, wfm, fr] = wfir("sb",127,[.2 .3],"hm",[0 0]);
    if is_handle_valid(my_handle) then
        clf(my_handle,"reset");
        demo_viewCode("winfilt.dem.sce");
        plot2d(fr',log(wfm)')
        xtitle(" ", _("frequency"), _("magnitude"));
        xtitle([_("Windowed FIR Low pass Filter"); ..
        _("Hamming window, cut-off: 0.2, 0.3"); ..
        _("length 127")]);

        halt(_("Press Return to continue ... \n"));


    end

    // Band pass filter (length 55, Chebyshev window, cut-offs .15 and .35)
    [wft, wfm, fr] = wfir("bp",55,[.15 .35],"ch",[.001 -1]);
    if is_handle_valid(my_handle) then
        clf(my_handle,"reset");
        demo_viewCode("winfilt.dem.sce");
        plot2d(fr',log(wfm)')
        xtitle(" ", _("frequency"), _("magnitude"));
        xtitle([_("Windowed FIR Band pass Filter"); ..
        _("Chebyshev window, cut-off: 0.15, 0.35"); ..
        _("length 55")]);

        halt(_("Press Return to continue ... \n"));

        delete(my_handle);
    end

endfunction

demo_winfilt();
clear demo_winfilt;
