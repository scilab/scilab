function val=mtlb_realmin(prec)
// Copyright INRIA
// Emulation function for Matlab realmin()
// V.C.

rhs=argn(2);

if rhs<=0 then
  val=number_properties("tiny");
elseif rhs==1 then
  if prec=="double" then
    val=number_properties("tiny");
  else
    error(gettext("Not yet implemented."));
  end
else
  error(gettext("Wrong number of inputs."));
end
endfunction
