function scipad(f)
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
//    if TK_GetVar("isscipadopen")==0 then
      TK_EvalStr("scipad eval {source ""'+SCI+'/tcl/scipad.tcl""}")
//    end
    if argn(2)==1 then       
      if type(f)==10 then 
        TK_EvalStr('scipad eval {openfile '+pathconvert(f,%f,%t)+'}')
// close "Untitled.sce" opened as default when scipad is started anew
        if TK_GetVar("isscipadopen")=='0' then
          TK_EvalStr('scipad eval {closefile $pad.textarea}')
        end
      end
    end
  else
    error(' Scilab has not been built with tk: scipad unavailable')
  end
endfunction

