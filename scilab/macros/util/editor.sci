function editor()
global LANGUAGE
[lhs,rhs]=argn(0)
if rhs<>0 then error(39), end
if MSDOS then
  if funptr("TK_EvalStr")==0 then
    write(%io(2),["Editor works only with TCL/TK.";],"(a)")
    return
  end
  TK_EvalStr("scipad eval {set lang "+LANGUAGE+"}")
  TK_EvalStr("scipad eval {set SciPath """+SCI+"""}")
  TK_EvalStr("scipad eval {source $SciPath/tcl/scipad.tcl}")
else
  write(%io(2),["Editor works only under Windows.";],"(a)")
end
endfunction
