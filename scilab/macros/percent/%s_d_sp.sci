function r=%s_d_sp(a,b)
//r=a./b
// Copyright INRIA
[ij,v,mn]=spget(b)
if size(v,'*')<>mn(1)*mn(2) then
  error(27)
else
  r=a./full(b)
end
endfunction
