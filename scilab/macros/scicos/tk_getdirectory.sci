function path=tk_getdirectory()
  TCL_EvalStr("set dirdd [tk_chooseDirectory -mustexist true]")
  path=stripblanks(TCL_GetVar('dirdd'))
  if path==emptystr() then path=[],end
endfunction                           