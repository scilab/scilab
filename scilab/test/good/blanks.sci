// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// blanks macro
// blanks(n)
// Create string of blank characters

function str = blanks(n)
	
	if ~or(type(n)==[1,8]) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: An integer expected.\n"),"blanks",1));
	end
	
	if and(size(n)<>[1 1]) then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: An integer expected.\n"),"blanks",1));
	end
	
	if n>0 then
		str = part(" ",ones(1,n));
	elseif n==0
		str = "";
	else
		error(msprintf(gettext("%s: Wrong values for input argument #%d: An non-negative integer expected.\n"),"blanks",1));
	end
	
endfunction
