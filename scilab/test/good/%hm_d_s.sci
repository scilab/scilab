function M1=%hm_d_s(M1,M2)
// Copyright INRIA
if size(M2,'*')<>1 then
  error('inconsistent element-wise operation')
else
  M1('entries')=M1('entries')./M2
end
endfunction
