function scipad(varargin)
// start scipad editor
global %scipad_fontsize;
global TMPDIR
  if with_tk() then
    TK_EvalStr("set isscipadinterp [interp exists scipad]")
    if TK_GetVar("isscipadinterp")=="0" then    
      TK_EvalStr("interp create scipad")
      TK_EvalStr("load {'+gettklib()+'} Tk scipad")
      TK_EvalStr("scipad eval {wm withdraw .}")
      TK_EvalStr("scipad alias ScilabEval ScilabEval")
    end
    if exists("LANGUAGE") then 
      TK_EvalStr("scipad eval { set lang """+LANGUAGE+""" }")
    end
    if MSDOS then 
      TK_EvalStr("scipad eval { set tmpdir """+strsubst(TMPDIR,"\","/")+""" }")
    else
      TK_EvalStr("scipad eval { set tmpdir """+pathconvert(TMPDIR,%f,%t)+""" }")
    end
    TK_EvalStr("scipad eval { set FontSize """+string(%scipad_fontsize)+""" }")
    TK_EvalStr("set isscipadopen [scipad eval {info exists pad}]")
    TK_EvalStr("scipad eval {source ""'+SCI+'/tcl/scipad.tcl""}")
    nfiles=argn(2)
    if nfiles>0 then
      onevalidfile=%f
      for i=1:nfiles
        validfile=%f;
        f=varargin(i)
        select type(f)        
          case 1 then filetoopen=string(f); validfile=%t;
          case 8 then filetoopen=string(f); validfile=%t;
          case 10 then filetoopen=f; validfile=%t;
////nice try, but can't be done. The assignement to the argument
//// overrides the original function name, and this is correct.
//          case 13 then
//            b=macr2tree(f); funname=b(2) //DOESN'T WORK. How else?
//            libmacro=whereis(funname)
//                 disp(libmacro,funname)
//            if libmacro<>[] & funname<>[] then
//              a=string(eval(libmacro));libpath=a(1)
//              if libpath<>[] then
//                 validfile=%t;
//          //the correspondance between function and file name it is tacitly assumed
//                 filetoopen=pathconvert(libpath+funname+".sci",%f)
//              else
//                 warning("Function "+funname+"is not contained in a loaded library, "..
//                         +"scipad doesn''t know where to find its source")
//              end
//            end
          else
            warning("scipad cannot open a "+typeof(f)+" object!")
        end
        if validfile then 
          onevalidfile=%t
          if MSDOS then 
            filetoopen=strsubst(filetoopen,"\","/"); 
          else
            filetoopen=pathconvert(filetoopen,%f,%t);
          end
          TK_EvalStr("scipad eval {openfile """+ filetoopen +"""}")          
        end
      end
  // close "Untitled.sce" opened as default when scipad is started anew
      if TK_GetVar("isscipadopen")=="0" & onevalidfile then
          TK_EvalStr("scipad eval {closefile $pad.textarea}")
      end
    end
  else
    error(" Scilab has not been built with tk: scipad unavailable")
  end
endfunction

