// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_tclsci_puzzle()

    tkpath = SCI + "/modules/tclsci/demos/tk/";

    TCL_EvalFile(tkpath+"puzzle")
    while %t //wait for toplevel to disapear
        order = TCL_GetVar("order");
        TCL_EvalStr("set h [winfo exists .puzzle]");
        if TCL_GetVar("h")=="0" then break,end
        sleep(1);
    end
    disp(order);

endfunction

demo_tclsci_puzzle()
clear demo_tclsci_puzzle;