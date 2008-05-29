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
// hex2dec function
//
// hex2dec(h) returns in vector d the numbers corresponding to the 
// hexadecimal representation h.
//
// -Input :
//    str : a string (or a vector/matrix of strings)
// -Output :
//    y : a scalar/vector/matrix
//
// =============================================================================

function d=hex2dec(h)
	
	if type(h)<> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),"hex2dec",1));
	end
	
	[nr,nc] = size(h)
	n       = length(h)
	p       = cumprod([1,16*ones(1,max(n)-1)]);
	d       = zeros(h);
	
	for i=1:nr
		for j=1:nc
			s = abs(str2code(h(i,j)));
			if max(s)>15 then
				error(msprintf(gettext("%s: Wrong value for input argument #%d: Valid hexadecimal representations expected.\n"),"hex2dec",1));
			end
			d(i,j) = p(n(i,j):-1:1)*s;
		end
	end
	
endfunction
