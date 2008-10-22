// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function sciGUI_init()
  if execstr('TCL_GetVar(''sciGUITable(win,0,type)'')','errcatch')<>0 then
    sciGUI_tclpath=SCI+"/modules/tclsci/tcl/sciGUI/"
    TCL_EvalFile(sciGUI_tclpath+'sciGUI.tcl');
    TCL_EvalFile(sciGUI_tclpath+'sciBrowseVar.tcl');
    TCL_EvalFile(sciGUI_tclpath+'sciEditVar.tcl');
    TCL_EvalFile(sciGUI_tclpath+'sciConfig.tcl');
    TCL_EvalStr('sciGUIDoInit ""'+SCI+'/modules/tclsci'+'"";sciGUIBrowseVarINI;');
  end
endfunction
