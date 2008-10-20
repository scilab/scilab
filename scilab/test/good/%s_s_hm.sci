function M2=%s_s_hm(M1,M2)
// Copyright INRIA/Enpc 
// scalar -hypermatrix 
  if and(size(M1)==[1 1]) then
    M2.entries=M1-M2.entries
  else
    dims1=size(M1)';
    dims2=matrix(M2.dims,-1,1)
    dims1(3:size(dims2,'*'))=1
    if and(dims1==dims2) then //should not occur
       M2.entries=matrix(M1,-1,1)-M2.entries
     else
       error(9)
     end
  end
endfunction
