// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y=flipdim(x,dim)

// FLIPDIM function
// Given x, a scalar/vector/matix of reals and an integer dim, this function flips the x components  along the dimension number dim (x and y have the same size). 
// -Inputs :
//  x : a scalar/vector/array of reals
//  dim : a positive integer
// -Output :
//  y : a scalar/vector/array of reals
//
// F.Belahcene

if size(dim,"*")<>1 then
	error(msprintf(gettext("%s: Wrong size for input argument #%d: A positive integer expected.\n"),"flipdim",2));
elseif type(dim)<>8 & (type(dim)<>1 | dim<1) then
	error(msprintf(gettext("%s: Wrong type for input argument #%d: A positive integer expected.\n"),"flipdim",2));
end


if ndims(x)==2 & type(x)<>1 then
	error(msprintf(gettext("%s: Wrong value for input argument #%d: Real matrix expected.\n"),"flipdim",1));
elseif ndims(x)>2 & type(x.entries)<>1
	error(msprintf(gettext("%s: Wrong value for input argument #%d: Real matrix expected.\n"),"flipdim",1));
end

dim=floor(dim);

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
