function val=mtlb_realmax(prec)
// Copyright INRIA
// Emulation function for Matlab realmax()
// V.C.

rhs=argn(2);

if rhs<=0 then
  val=number_properties("huge");
elseif rhs==1 then
  if prec=="double" then
    val=number_properties("huge");
  else
    error(gettext("errors","compatibility_functions_error_2"));
  end
else
  error(gettext("errors","compatibility_functions_error_58"));
end
endfunction
