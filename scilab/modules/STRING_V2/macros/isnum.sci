// =============================================================================
// Author : Pierre MARECHAL
// Scilab Project
// Copyright INRIA 2007
// Date : 18 Dec 2007
//
// isnum(str) return %T if str represent a number
//
// =============================================================================

function res = isnum(str)
	
	if type(str) <> 10 then
		error(44,1);
	end
	
	[nr,nc] = size(str);
	
	for j=1:nc
		for i=1:nr
			res(i,j) = (grep(str(i,j),"/^([+-]?(([0-9]+)?\.)?[0-9]+([eEdD][-+]?[0-9]+)?)$/","r") == 1);
			if str(i,j) == '%pi' then
				res(i,j) = %T;
			end
		end
	end
	
endfunction

