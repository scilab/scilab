// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function result = emptystr(varargin)
	[lhs, rhs] = argn(0);

	if rhs == 1 then
		//emptystr(x)
		s = size(varargin(1));
		p = prod(s);
		if p == 0 then
			//emptystr([])
			result = [];
		else
			result(1:p) = "";
			result = matrix(result, s);
		end
	elseif rhs == 2 then
		//emptystr(x,y)
		x = varargin(1);
		y = varargin(2);

		if type(x) <> 1 then
		  error(msprintf(_("%s: Wrong type for input argument #%d: Matrix of integers expected.\n"), "emptystr", 1));
		end
		if type(y) <> 1 then
		  error(msprintf(_("%s: Wrong type for input argument #%d: Matrix of integers expected.\n"), "emptystr", 2));
		end

		if x == 0 | y == 0 then
			result = [];
		else
			result(x,y) = "";
		end
	else
	//emptystr() or emptystr(x,y, ... )
		result = "";
	end
endfunction

