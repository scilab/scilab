function M1=%hm_a_hm(M1,M2)
// Copyright INRIA
// hypermatrix addition
if or(M1.dims<>M2.dims) then
  error(8)
end
M1.entries=M1.entries+M2.entries
endfunction
