function x=addfv(x1,x2)
// Copyright INRIA
// développé par EADS-CCR
  if (x1 == [] ) then
    x=x2;
  elseif (x2 == []) then
    x=x1;
  else
    x=addf(x1,x2);
  end
endfunction
