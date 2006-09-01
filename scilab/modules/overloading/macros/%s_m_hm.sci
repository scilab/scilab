function M2=%s_m_hm(M1,M2)
// Copyright INRIA
if size(M1,'*')<>1 then
  M2=hypermat(size(M1),M1)*M2
else
  M2.entries=M1*M2.entries
end
endfunction
