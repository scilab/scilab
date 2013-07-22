// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_bode()

    //////////////////
    //BODE PLOTS//////
    //////////////////

    //First plot using state space representation
    //
    //   xd=-2*%pi*x+u
    //  y=18*%pi*x+u
    //
    // h(s)=18*%pi/(s+2*%pi)
    //
    a = -2*%pi;
    b = 1;
    c = 18*%pi;
    d = 1;
    sl = syslin("c",a,b,c,d);

    my_handle = scf(100001);
    if is_handle_valid(my_handle) then
        clf(my_handle,"reset");
        bode(sl,.1,100)
        halt(_("Press Return to continue ... \n"));
    end

    //Second plot using poly
    s=poly(0,"s");
    h1=1/real((s+2*%pi*(15+100*%i))*(s+2*%pi*(15-100*%i)));
    h1=syslin("c",h1)
    if is_handle_valid(my_handle) then
        clf(my_handle,"reset");
        bode(h1,10,1000,.01),
        halt(_("Press Return to continue ... \n"));
    end

    //Third plot which combines the first two plots
    if is_handle_valid(my_handle) then
        clf(my_handle,"reset");
        bode([h1;sl],.1,1000,.01);
        halt(_("Press Return to continue ... \n"));
    end

    // series connection
    if is_handle_valid(my_handle) then
        clf(my_handle,"reset");
        demo_viewCode("bode.dem.sce");
        bode(h1*sl,.1,1000,.01);
    end

endfunction

demo_bode();
clear demo_bode;
