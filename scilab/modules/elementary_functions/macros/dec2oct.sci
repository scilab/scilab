// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// =============================================================================
// Author : Pierre MARECHAL
// dec2oct function
// =============================================================================

function y = dec2oct(x)
	
	rhs = argn(2);
	
	// check the number of input arguments
	if rhs<>1 then
		error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"dec2oct",1));
	end
	
	if or(type(x)<>8) & (or(type(x)<>1) | or(x<0) ) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: Scalar/vector/matrix of positive integers expected.\n"),"dec2oct",1));
	end
	
	[nr,nc] = size(x);
	y       = emptystr(nr,nc);
	
	for i=1:nr
		for j=1:nc
			
			if x(i,j) < 8 then
				y(i,j) = string(x(i,j));
				continue;
			end
			
			x_bin          = dec2bin(x(i,j));
			x_bin_length   = length(x_bin);
			
			// Add 0 to the right of the string
			while modulo(length(x_bin),3) != 0 ,
				x_bin = '0' + x_bin;
			end
			
			x_bin_splitted = strsplit(x_bin,3*[1:(length(x_bin)/3-1)]);
			y(i,j)         = code2str(bin2dec(x_bin_splitted));
			
		end
	end
	
endfunction
