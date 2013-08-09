// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_tclsci_scroll()

    mprintf("\n");

    tkpath = SCI + "/modules/tclsci/demos/tk/";

    TCL_EvalFile(tkpath + "cscroll")

    while %t //wait for toplevel to disapear
        TCL_EvalStr("set h [winfo exists .cscroll]");
        if TCL_GetVar("h")=="0" then break,end
        sleep(1);
    end

    mprintf("\n");

endfunction

demo_tclsci_scroll();
clear demo_tclsci_scroll;
