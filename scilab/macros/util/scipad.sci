function scipad(varargin)
// start scipad editor
  if with_tk() then
    TK_EvalStr("set isscipadinterp [interp exists scipad]")
    if TK_GetVar("isscipadinterp")=='0' then    
      TK_EvalStr("interp create scipad")
      TK_EvalStr("load {'+gettklib()+'} Tk scipad")
      TK_EvalStr("scipad eval {wm withdraw .}")
      TK_EvalStr("scipad alias ScilabEval ScilabEval")
    end
    if exists("LANGUAGE") then 
         TK_EvalStr("scipad eval { set lang """+LANGUAGE+""" }")
    end
    TK_EvalStr("set isscipadopen [scipad eval {info exists pad}]")
    TK_EvalStr("scipad eval {source ""'+SCI+'/tcl/scipad.tcl""}")
    nfiles=argn(2)
    if nfiles>0 then
      onevalidfile=%f
      for i=1:nfiles
        f=varargin(i)       
        if type(f)==10 then
            onevalidfile=%t 
// caveat: this fails for eval(f)=integer, though
            TK_EvalStr('scipad eval {openfile '+pathconvert(f,%f,%t)+'}')
        end
      end
// close "Untitled.sce" opened as default when scipad is started anew
      if TK_GetVar("isscipadopen")=='0' & onevalidfile then
          TK_EvalStr('scipad eval {closefile $pad.textarea}')
      end
    end
  else
    error(' Scilab has not been built with tk: scipad unavailable')
  end
endfunction

