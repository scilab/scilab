// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_tclsci_scale()

    tkpath = SCI + "/modules/tclsci/demos/tk/";
    exec(tkpath + "/demredraw.sci");
    my_handle = scf(100001);
    clf(my_handle,"reset");
    plot3d();
    handle_demo_scale = gcf();

    done = %t;
    TCL_EvalFile(tkpath+"/vscale");
    while %t //wait for toplevel to disapear
        TCL_EvalStr("set h [winfo exists .vscale]");
        if TCL_GetVar("h")=="0" | ~is_handle_valid(handle_demo_scale) then
            break;
        end
        sleep(1)
    end

endfunction

demo_tclsci_scale();
clear demo_tclsci_scale;
