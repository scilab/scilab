function sciGUI_init()
  if execstr('TCL_GetVar(''sciGUITable(win,0,type)'')','errcatch')<>0 then
    sciGUI_tclpath=SCI+"/tcl/sciGUI/'
    TCL_EvalFile(sciGUI_tclpath+'sciGUI.tcl');
    TCL_EvalFile(sciGUI_tclpath+'sciBar.tcl');
    TCL_EvalFile(sciGUI_tclpath+'sciDemoPlay.tcl');
    TCL_EvalFile(sciGUI_tclpath+'sciBrowseVar.tcl');
    TCL_EvalFile(sciGUI_tclpath+'sciBrowseHelp.tcl');
    TCL_EvalFile(sciGUI_tclpath+'sciEditVar.tcl');
    TCL_EvalFile(sciGUI_tclpath+'sciConfig.tcl');
    TCL_EvalStr('sciGUIDoInit ""'+SCI+'"";sciGUIBrowseVarINI;sciGUIBrowseHelpINI');
  end
endfunction
