function M1=%hm_a_s(M1,M2)
// Copyright INRIA
// hypermatrix + scalar

if and(size(M2)==[1 1]) then
  M1.entries=M1.entries+M2
else
  dims1=matrix(M1.dims,-1,1)
  dims2=size(M2)';
  dims2(3:size(dims1,'*'))=1;
  if and(dims1==dims2) then //should not occur
    M1.entries=M1.entries+matrix(M2,-1,1)
  else
    error(8)
  end
end
endfunction
