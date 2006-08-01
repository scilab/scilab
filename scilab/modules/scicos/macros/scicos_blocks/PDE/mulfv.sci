function x=mulfv(x1,x2)
// Copyright INRIA
// développé par EADS-CCR
  if (x1 == [] | x2 == [] | x1 == '' | x2 == '') then
    x='0';
  else
    x=mulf(x1,x2);
  end
endfunction
