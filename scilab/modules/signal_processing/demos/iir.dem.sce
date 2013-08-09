// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_iir()

    ystr=[ "type (lp, hp, bp, sb) ";
    "order (integer)";
    "design (butt, cheb1, cheb2 , ellip)" ;
    "cut-off frequencies (2-vector in [0, 0.5])";
    "error values  0 delta(1),delta(2) 1 "];

    w = x_mdialog(_("Choose filter type and parameters"), ..
    ystr, ["bp";"3";"ellip";"0.15 0.25";"0.08 0.035"]);
    if w <> [] then
        hz = iir(evstr(w(2)),w(1),w(3),evstr(w(4)),evstr(w(5)));
        [hzm,fr] = frmag(hz, 256);

        my_handle = scf(100001);
        if is_handle_valid(my_handle) then
            clf(my_handle, "reset");
            plot2d(fr', hzm');
            xtitle(_("Discrete IIR filter ") + "( "+ w(3)+" ) ", " ", " ");
        end
    end

endfunction

demo_iir();
clear demo_iir;
