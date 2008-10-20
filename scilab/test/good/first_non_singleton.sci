function dim=first_non_singleton(m2scitlist)
// Copyright INRIA
dim=-1
if is_a_scalar(m2scitlist) | is_a_vector(m2scitlist) then
  dim=0;
  return
end
for k=1:size(m2scitlist.dims)
  if m2scitlist.dims(k)==0 then
    dim=0
    break
  elseif m2scitlist.dims(k)>0 & m2scitlist.dims(k)<>1 then
    dim=k
    break
  end
end

endfunction
