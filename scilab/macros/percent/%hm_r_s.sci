function M1=%hm_r_s(M1,M2)
// Copyright INRIA
if size(M2,'*')<>1 then
  M1=M1/hypermat(size(M2),M2)
else
  M1.entries=M1.entries/M2
end
endfunction
