// =============================================================================
// Author : Pierre MARECHAL
// Scilab team
// Copyright INRIA
// Date : 28 Dec 2005
//
// Return True if year is a leap year, otherwise False

// Si l'année est divisible par 4 et non par 100 => Année bissextile
// Si l'année est divisible par 400 => Année bissextile
// =============================================================================

function Rep=isLeapYear(year)
	
	lhs=argn(1);
	rhs=argn(2);
	
	if rhs==1 & type(year)==1 then
		Rep = ((modulo(year,100)<>0) & (modulo(year,4)==0)) | (modulo(year,400)== 0);
	else
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"isLeapYear",1));
	end
	
endfunction
