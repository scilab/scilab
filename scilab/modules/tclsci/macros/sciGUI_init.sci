// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function sciGUI_init()
    if execstr("TCL_GetVar(''sciGUITable(win,0,type)'')","errcatch")<>0 then
        sciGUI_tclpath=SCI+"/modules/tclsci/tcl/sciGUI/"
        TCL_EvalFile(sciGUI_tclpath+"sciGUI.tcl");
        TCL_EvalFile(sciGUI_tclpath+"sciBrowseVar.tcl");
        TCL_EvalFile(sciGUI_tclpath+"sciEditVar.tcl");
        TCL_EvalStr("sciGUIDoInit """+SCI+"/modules/tclsci"+""";sciGUIBrowseVarINI;");
    end
endfunction
