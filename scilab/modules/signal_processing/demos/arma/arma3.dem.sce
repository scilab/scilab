// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_arma3()

    mprintf("\n")
    mprintf(_("Arma Spectral power estimation:"))
    mprintf("\n")

    // Spectral power estimation
    // ( form Sawaragi et all)
    m = 18;
    a = [1,-1.3136,1.4401,-1.0919,+0.83527];
    b = [0.0,0.13137,0.023543,0.10775,0.03516];
    u = rand(1,1000,"n");
    z = arsimul(a,b,[0],0,u);

    //----Using macro mese
    [sm, fr] = mese(z, m);

    //----The theorical result
    function gx = gxx(z,a,b)
        w = exp(-%i*2*%pi*z*(0:4))'
        gx = abs(b*w)^2/(abs(a*w)^2);
    endfunction

    res = [];
    for x = fr,
        res = [ res, gxx(x,a,b)];
    end;

    //----using armax estimation of order  (4,4)
    // it's a bit tricky because we are not supposed to know the order
    [arc, la, lb, sig, resid] = armax(4, 4, z, u);
    res1 = [];
    for x = fr,
        res1 = [ res1, gxx(x,la(1),lb(1))];
    end;


    //-- visualization of the results

    my_handle = scf(100001);
    if is_handle_valid(my_handle) then
        clf(my_handle, "reset");
        plot2d([fr;fr;fr]', [20*log10(sm/sm(1));20*log10(res/res(1));20*log10(res1/res1(1))]', [2,1,-1])
        legend([_("Using macro mese"); _("Theoretical value"); _("Arma identification")])
        xtitle(_("Spectral power"), _("frequency"), _("spectral estimate"))
        demo_viewCode("arma3.dem.sce");
    end

endfunction

demo_arma3();
clear demo_arma3;
