function M2=%s_x_hm(M1,M2)
// Copyright INRIA
if size(M1,'*')<>1 then
  error('inconsistent element-wise operation')
else
  M2('entries')=M1.*M2('entries')
end
endfunction
