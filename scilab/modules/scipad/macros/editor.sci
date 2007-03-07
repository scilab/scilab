function editor()
  [lhs,rhs]=argn(0)
  if rhs<>0 then error(39), end // (??)
  if (~%tk)
    write(%io(2),[gettext('messages','scipad_message_7');],"(a)")
  return
  else
    scipad()
  end
endfunction
