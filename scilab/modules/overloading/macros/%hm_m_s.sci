function M1=%hm_m_s(M1,M2)
// Copyright INRIA
if size(M2,'*')<>1 then
  M1=M1*mlist(['hm','dims','entries'],size(M2),matrix(M2,-1,1))
else
  M1.entries=M1.entries*M2
end
endfunction
