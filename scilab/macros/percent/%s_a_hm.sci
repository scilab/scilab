function M2=%s_a_hm(M1,M2)
// Copyright INRIA
// hypermatrix + scalar
if and(size(M1)==[1 1]) then
  M2.entries=M1+M2.entries
else
  dims=M2.dims(:)
  dims(find(dims==1))=[]
  if and(dims'==size(M1)) then
    M2.entries=M1(:)+M2.entries
  else
    error(9)
  end
end





