function r=%sp_q_s(a,b)
//r=a.\b
// Copyright INRIA
[ij,v,mn]=spget(a)
if size(v,'*')<>mn(1)*mn(2) then
  error(27)
else
  r=full(a).\b
end
endfunction
