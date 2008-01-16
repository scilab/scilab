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
		error(gettext("Both parameters must have the same size."));
	end
	
	if rhs <> 2 then
		error(gettext("Number of parameters incorrect."));
	end

	if (type(Y) <> 1) | (type(M) <> 1) then
		error(gettext("Parameters must be integers."));
	end
	
	if (int(Y)<>Y) | (int(M)<>M) then
		error(gettext("Parameters must be integers."));
	end
	
	if (min(M) < 1) | (max(M) > 12) then
		error(gettext("The second parameter must be between 1 and 12."));
	end
	
	[nr,nc] = size(M);
	
	E(  isLeapYear(Y) ) = leap_year(   M( isLeapYear(Y)) );
	E( ~isLeapYear(Y) ) = common_year( M(~isLeapYear(Y)) );
	
	E = matrix(E,nr,nc);
	
endfunction
