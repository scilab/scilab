//---------------------------------------------------------------------------------------------------------------------
// Author : Pierre MARECHAL
// Scilab team
// Copyright INRIA 
// Date : 29 Dec 2005
//
// Returns the last day of the year and month by corresponding element of Matrix Y and M
//---------------------------------------------------------------------------------------------------------------------

function E=eomday(Y,M)

	rhs=argn(2);
	
	normal_year = [31,28,31,30,31,30,31,31,30,31,30,31];
	leap_year = [31,29,31,30,31,30,31,31,30,31,30,31];
	
	if size(Y) <> size(M) then
		error("Both parameters must have the same size.");
	end
	
	if rhs <> 2 then
		error("Number of parameters incorrect.");
	end
	
	[nr,nc] = size(Y);
	E = ones(nr,nc);
	
	for i=1:nr
		for j=1:nc
			
			if (type(Y(i,j)) <> 1) | (type(M(i,j)) <> 1) then
				error("Parameters must be integers");
			end
			
			if (floor(Y(i,j))<>Y(i,j)) | (floor(M(i,j))<>M(i,j)) then
				error("Parameters must be integers");
			end
			
			if (M(i,j) < 1) | (M(i,j) > 12) then
				error("The second parameter must be between 1 and 12");
			end
			
			if isLeapYear(Y(i,j)) then
				E(i,j) = leap_year(M(i,j));
			else
				E(i,j) = normal_year(M(i,j));
			end
		end
	end

endfunction
