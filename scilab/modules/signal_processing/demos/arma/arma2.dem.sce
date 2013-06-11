// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_arma2()

    editor SCI/modules/signal_processing/demos/arma/arma2.dem.sce;
    l = lines();
    lines(0);

    // A bidimensional version of demo one.
    a = [1,-2.851,2.717,-0.865].*.eye(2,2);
    b = [0,1,1,1].*.[1;1];
    d = [1,0.7,0.2].*.eye(2,2);
    sig = eye(2,2);
    ar = armac(a,b,d,2,1,sig);
    disp(_("Simulation of an ARMAX process: "));
    disp(ar);

    u = -prbs_a(300,1,int([2.5,5,10,17.5,20,22,27,35]*100/12));
    zd = narsimul(a,b,d,sig,u);
    z = narsimul(a,b,d,0.0*sig,u);
    disp(_("Identification ARX (least square): "));
    [archap,la,lb,sig,resid]=armax(3,3,zd,u,1,1);
    disp(archap);

    lines(l(2), l(1));

endfunction

demo_arma2();
clear demo_arma2;
