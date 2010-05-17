// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// =============================================================================
//
// base2dec function
//
// base2dec(h,b) Convert base b string to decimal integer
//
// - Input :
//    h : a string (or a vector/matrix of strings)
//    b : an integer
//
// - Output :
//    y : a scalar/vector/matrix
//
// =============================================================================

function [d]=base2dec(h,b)
	
	if type(h)<> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),"base2dec",1));
	end
	
	if (type(b)<>1) & (size(b,"*")<>1) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: integer expected.\n"),"base2dec",2));
	end
	
	if (b<2) | (b>36) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d."),"base2dec",2,2,36));
	end
	
	[nr,nc] = size(h)
	n       = length(h)
	p       = cumprod([1,b*ones(1,max(n)-1)]);
	d       = zeros(h);
	
	for i=1:nr
		for j=1:nc
			s = abs(str2code(h(i,j)));
			if max(s)>(b-1) then
				error(msprintf(gettext("%s: Wrong value for input argument(s): Valid base %d representations expected.\n"),"base2dec",12));
			end
			d(i,j) = p(n(i,j):-1:1)*s;
		end
	end
	
endfunction
