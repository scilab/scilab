//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is distributed under the same license as the Scilab package.
//

function demo_datafit()

    function y = FF(x)
        // parametric function model
        y = a*(x-b)+c*x.*x;
    endfunction

    function e = G(p, z)
        // datafit external computes the error
        a = p(1),
        b = p(2),
        c = p(3),
        y = z(1),
        x = z(2),
        e = y - FF(x)
    endfunction

    // create the experimental data
    X = [];
    Y = [];
    a = 34;
    b = 12;
    c = 14;
    for x=0:.1:3, Y=[Y,FF(x)+100*(rand()-.5)];X=[X,x];end
    Z = [Y; X];
    //show the data points
    my_handle = scf(100001);
    clf(my_handle, "reset");
    demo_viewCode(SCI + "/modules/optimization/demos/datafit/demo_datafit.sci");
    plot(X, Y, "+");
    f = gcf();
    l=legend(_("Experimental data"),2);
    sleep(500);
    // solve the non linear data fitting
    [p,err] = datafit(G,Z,[3;5;10]);
    if is_handle_valid(f) then // If the window is still open after the sleep
        // show the fitting curve
        drawlater()
        plot(X,FF(X), "r")
        delete(l);
        l = legend([_("Experimental data"); _("Fitting function")],2);
        drawnow()
    end

endfunction

