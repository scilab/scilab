function M=hypermat(dims,v)
// Copyright INRIA
//initialize an hypermatrix whose dimensions are given in the vector dims
// all entries are set to 0
//
// M data structure contains the vector of matrix dimensions M('dims')
// and the vector of entries M('entries') such as the leftmost subcripts vary first
// [M(1,1,..);..;M(n1,1,..);...;M(1,n2,..);..;M(n1,n2,..);...]
[lhs,rhs]=argn(0)
//dims=round(dims)
if rhs<2 then
  M=mlist(['hm','dims','entries'],matrix(dims,1,-1),zeros(prod(dims),1))
else
  if size(v,'*')<> double(prod(dims)) then
    error('hypermat: Number of entries does not match product of dimensions')
  end
  M=mlist(['hm','dims','entries'],matrix(dims,1,-1),v(:))
end
endfunction
