//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is distributed under the same license as the Scilab package.
//

function demo_tclsci_rolodex()
    tkpath = SCI + "/modules/tclsci/demos/tk/";
    TCL_EvalFile(tkpath+"rolodex")
    while %t //wait for toplevel to disapear
        TCL_EvalStr("set h [winfo exists .top]");
        if TCL_GetVar("h")=="0" then break,end
        sleep(1);
    end
endfunction

demo_tclsci_rolodex()
clear demo_tclsci_rolodex;
