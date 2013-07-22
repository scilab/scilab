// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

////////////////////////
//MINIMAX FIR FILTERS///
////////////////////////


function demo_remezfilter()

    // Low pass filter (length 33, cut-off in [.23,.27], weight 1/.1)

    hn = eqfir(33,[0 .23;.27 .5],[1 0],[1 .1]);
    [hm, fr] = frmag(hn, 256);

    my_handle = scf(100001);
    if is_handle_valid(my_handle) then
        clf(my_handle, "reset");
        plot2d(fr', hm')
        title_c = [_("Minimax FIR filter");
        _("low pass  cut-off in [.23,.27]");
        _("length 33")];
        ax = gca();
        ax.margins(3) = 0.2;
        title(title_c);

        halt(_("Press Return to continue ... \n"));
    end

    //Triangular filter created using remezb
    nc = 21;
    ngrid = nc * 16;
    fg = .5*(0:(ngrid-1))/(ngrid-1);
    ds(1:ngrid/2) = (0:-1+ngrid/2)*2/(ngrid-2);
    ds(ngrid/2+1:ngrid) = ds(ngrid/2:-1:1);
    wt = ones(fg);
    [an] = remezb(nc, fg, ds, wt);
    hn(1:nc-1) = an(nc:-1:2)/2;
    hn(nc) = an(1);
    hn(nc+1:2*nc-1) = an(2:nc)/2;
    [hm, fr] = frmag(hn, max(size(ds)));

    if is_handle_valid(my_handle) then
        clf(my_handle, "reset");
        plot2d(fr', hm'),
        xtitle(_("Remez Triangular filter"), _("frequency"), ("magnitude"));

        halt(_("Press Return to continue ... \n"));
    end

    if is_handle_valid(my_handle) then
        clf(my_handle, "reset");
        demo_viewCode("remezfilt.dem.sce");
        plot2d(fr',( hm-ds)')
        xtitle(" ", _("frequency"), _("magnitude error"));
    end

endfunction

demo_remezfilter();
clear demo_remezfilter;
