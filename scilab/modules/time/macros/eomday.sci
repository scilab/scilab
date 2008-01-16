//------------------------------------------------------------------------------------------------------------
// Author : Pierre MARECHAL
// Scilab team
// Copyright INRIA
// Date : 29 Dec 2005
//
// Returns the last day of the year and month by corresponding element of Matrix Y and M
//------------------------------------------------------------------------------------------------------------

function E=eomday(Y,M)

	rhs=argn(2);
	
	common_year = [31,28,31,30,31,30,31,31,30,31,30,31];
	leap_year   = [31,29,31,30,31,30,31,31,30,31,30,31];
	
	if size(Y) <> size(M) then
		error(msprintf(gettext("%s: Wrong size for input argument: same size expected.\n"),"eomday"));
	end
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments.\n"),"eomday"));
	end

	if (type(Y) <> 1) | (type(M) <> 1) then
		error(msprintf(gettext("%s: Wrong type for input arguments: Integer expected.\n"),"eomday"));
	end
	
	if (int(Y)<>Y) | (int(M)<>M) then
		error(msprintf(gettext("%s: Wrong type for input arguments: Integer expected.\n"),"eomday"));
	end
	
	if (min(M) < 1) | (max(M) > 12) then
		error(msprintf(gettext("%s: Wrong value for second input argument: Must be between 1 and 12.\n"),"eomday"));
	end
	
	[nr,nc] = size(M);
	
	E(  isLeapYear(Y) ) = leap_year(   M( isLeapYear(Y)) );
	E( ~isLeapYear(Y) ) = common_year( M(~isLeapYear(Y)) );
	
	E = matrix(E,nr,nc);
	
endfunction
