function f=%r_l_p(f,m)
// f\m
//!
// Copyright INRIA
if size(f('num'),'*')<>1 then 
  f=invr(f)*m
else
  f=rlist(m*f('den'),ones(m)*f('num'),f('dt'))
end
endfunction
