// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004 - INRIA
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// This function returns TRUE if "field" is a field of the struct "s", FALSE otherwise

function r = isfield(s,field)
	
	r = [];
	
	rhs = argn(2);
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"isfield",2));
	end
	
	if ~isstruct(s) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: struct array expected.\n"),"isfield",1));
	end
	
	if type(field) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"isfield",2));
	end
	
	[nr,nc]     = size(field);
	fields      = getfield(1,s);
	fields(1:2) = [];
	
	for row=1:nr
		for line=1:nc
			r(row,line) = or(fields==field(row,line));
		end
	end
	
endfunction
