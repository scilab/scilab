function scipad(varargin)
  // start scipad editor
if with_tk() then
  TK_EvalStr("set isscipadinterp [interp exists scipad]")

  if MSDOS then
    if TK_GetVar("isscipadinterp")=='0' then    
      TK_EvalStr("interp create scipad")
            TK_EvalStr("load {tk83.dll} Tk scipad")
      TK_EvalStr("scipad eval {wm withdraw .}")
      TK_EvalStr("scipad alias ScilabEval ScilabEval")
    end
    TK_EvalStr("scipad eval {source ""'+SCI+'/tcl/scipad.tcl""}")
  else
    if TK_GetVar("isscipadinterp")=='0' then   
      TK_EvalStr("interp create scipad")
      TK_EvalStr("load {libtk8.3.so} Tk scipad")
      TK_EvalStr("scipad eval {wm withdraw .}")
      TK_EvalStr("scipad alias ScilabEval ScilabEval")
    end
    TK_EvalStr("scipad eval {source '+SCI+'/tcl/scipad.tcl}")

  end
else
  error(' Scilab has not been built with tk: scipad unavailable')
end
endfunction
