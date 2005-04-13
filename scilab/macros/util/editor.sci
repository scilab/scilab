function editor()
  [lhs,rhs]=argn(0)
  if rhs<>0 then error(39), end // (??)
  if funptr("TCL_EvalStr")==0 then
    write(%io(2),["Editor works only with TCL/TK.";],"(a)")
    return
  else
    scipad()
  end
endfunction
