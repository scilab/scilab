function sciGUI_init()
  if execstr('TK_GetVar(''sciGUITable(win,0,type)'')','errcatch')<>0 then
    sciGUI_tclpath=SCI+"/tcl/sciGUI/'
    TK_EvalFile(sciGUI_tclpath+'sciGUI.tcl');
    TK_EvalFile(sciGUI_tclpath+'sciBar.tcl');
    TK_EvalFile(sciGUI_tclpath+'sciDemoPlay.tcl');
    TK_EvalFile(sciGUI_tclpath+'sciBrowseVar.tcl');
    TK_EvalFile(sciGUI_tclpath+'sciBrowseHelp.tcl');
    TK_EvalFile(sciGUI_tclpath+'sciEditVar.tcl');
    TK_EvalFile(sciGUI_tclpath+'sciConfig.tcl');
    TK_EvalStr('sciGUIDoInit ""'+SCI+'"";sciGUIBrowseVarINI;sciGUIBrowseHelpINI');
  end
endfunction
