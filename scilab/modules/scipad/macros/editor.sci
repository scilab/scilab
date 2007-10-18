function editor()
  [lhs,rhs]=argn(0)
  if rhs<>0 then error(39), end // (??)
  if (~%tk)
    write(%io(2),[gettext('Editor works only with Tcl/Tk.');],"(a)")
  return
  else
    scipad()
  end
endfunction
