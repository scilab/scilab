function M2=%hm_d_hm(M1,M2)
// Copyright INRIA
if and(M1('dims')==M2('dims')) then
  M2('entries')=M1('entries')./M2('entries')
else
 error('inconsistent element-wise operation')
end
endfunction
