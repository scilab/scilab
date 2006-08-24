function y=flipdim(x,dim)
// Copyright INRIA
// FLIPDIM function
// Given x, a scalar/vector/matix of reals and an integer dim, this function flips the x components  along the dimension number dim (x and y have the same size). 
// -Inputs : 
//  x : a scalar/vector/array of reals
//  dim : a positive integer
// -Output :
//  y : a scalar/vector/array of reals
//
// F.Belahcene

if or(size(dim)<>[1 1]) | type(dim)<>8 & (type(dim)<>1 | dim<1) then
	error("second input argument must be a positive scalar")
end

if ndims(x)==2 & type(x)<>1 then
	error("first input argument must be a scalar/vector/array of reals")
elseif ndims(x)>2 & type(x.entries)<>1
	error("first input argument must be a scalar/vector/array of reals")
end

dim=floor(dim)
if dim>ndims(x)
	y=x
	return
end

l=list()
for k=1:dim-1
	l(k)=eye()
end
	l(dim)=$:-1:1
for k=dim+1:ndims(x)
	l(k)=eye()
end

y=x(l(:))

endfunction