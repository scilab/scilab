function scipad(f)
// start scipad editor
  if with_tk() then
    TK_EvalStr("set isscipadinterp [interp exists scipad]")
    if TK_GetVar("isscipadinterp")=='0' then    
      TK_EvalStr("interp create scipad")
      if MSDOS then tklib='tk83.dll', else tklib='libtk8.3.so',end
      TK_EvalStr("load {'+tklib+'} Tk scipad")
      TK_EvalStr("scipad eval {wm withdraw .}")
      TK_EvalStr("scipad alias ScilabEval ScilabEval")
    end
    TK_EvalStr("scipad eval {source ""'+SCI+'/tcl/scipad.tcl""}")
    if argn(2)==1 then 
      TK_EvalStr('scipad eval {openfile '+f+'}')
    end
  else
    error(' Scilab has not been built with tk: scipad unavailable')
  end
endfunction
