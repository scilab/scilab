function M2=%s_s_hm(M1,M2)
// Copyright INRIA/Enpc 
// scalar -hypermatrix 
  if and(size(M1)==[1 1]) then
    M2.entries=M1-M2.entries
  else
     dims=matrix(M2.dims,-1,1)
     dims(find(dims==1))=[]
     if and(dims'==size(M1)) then
       M2.entries=matrix(M1,-1,1)-M2.entries
     else
       error(9)
     end
  end
endfunction
