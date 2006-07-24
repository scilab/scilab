//------------------------------------------------------------------------
// Author : Pierre MARECHAL
// Scilab team
// Copyright INRIA 
// Date : 28 Dec 2005
//
// Return True if year is a leap year, otherwise False 
//------------------------------------------------------------------------

function Rep=isLeapYear(year)
	
	lhs=argn(1);
	rhs=argn(2);
	
	select rhs
		case 1
			
			// Si l'année est divisible par 4 et non par 100 => Année issextile
			// Si l'année est divisible par 400 => Année issextile
			
			Rep=%F
			if modulo(year,100)<>0 then
				r = modulo(year,4);
				if r == 0 then
					Rep=%T
				end
			else
				r = modulo(year,400);
				if r == 0 then
					Rep=%T
				end
			end
		else
			error('Number of parameters incorrect.');
		end
	
endfunction
