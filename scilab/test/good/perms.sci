function y=perms(x)
// Copyright INRIA
// PERMS function
// Given a vector x (size: 1*n or n*1), this function returns all the permutations of x (i.e n! permutations)
// Inputs : 
//  x : scalars/vectors
// output :
// y : matrix
// F.Belahcene

if size(x,"*")==1 | size(x,"*")==0 then
  y=x
elseif size(x,1)<>1 & size(x,2)<>1 then
  error("input argument must be a vector")
else
  xsize=size(x,"*")
  indrec=perms(1:xsize-1)
  ind=[ones(prod(1:xsize-1),1)*xsize,indrec]
  for j=xsize-1:-1:1
    indrec(indrec==j)=j+1
    ind=[ind;[ones(prod(1:xsize-1),1)*j,indrec]]
  end
  y=matrix(x(ind),size(ind))
end

endfunction

