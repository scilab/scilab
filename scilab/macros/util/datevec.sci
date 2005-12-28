//------------------------------------------------------------------------
// Allan CORNET
// INRIA 2004
//
// Modified by Pierre MARECHAL
// Scilab team
// Copyright INRIA 
// Date : 28 Dec 2005
//------------------------------------------------------------------------

function [Y,M,D,h,m,s] = datevec(varargin)

	lhs=argn(1);   
	rhs=argn(2);
	
	normal_year = [0,31,59,90,120,151,181,212,243,273,304,334,365];
	bissextile_year = [0,31,60,91,121,152,182,213,244,274,305,335,366];
	
	if (rhs==1) & (size(varargin(1)) == [1,1]) then
		ValDate=varargin(1);
		Y = 0;
		M = ones(size(Y));
		D = ones(size(Y));
		
		// Get the year
		
		if ValDate == floor(ValDate) then
			h = 0; 
			m = 0;
			s = 0;
		else
			s = 86400*(ValDate-floor(ValDate));
			h = floor(s/3600);
			s = s - 3600*h;
			m = floor(s/60);
			s = s - 60*m;
		end
		
		ValDate = floor(ValDate);
		Y = floor(ValDate/365.2425);
		
		temp = ValDate - (365.0*Y + ceil(0.25*Y)- ceil(0.01*Y) + ceil(0.0025*Y));
		
		if  temp <= 0 then
			Y = Y - 1;
			ValDate = ValDate - (365.0*Y + ceil(0.25*Y) - ceil(0.01*Y) + ceil(0.0025*Y));
		else
			ValDate = temp;
		end
		
		M = int (ValDate/29.);
		
		if isLeapYear(Y) then
			if ValDate > bissextile_year(M+1) then
				M = M+1;
			end
			D = ValDate - bissextile_year(M);
		else
			if ValDate > normal_year(M+1) then
				M = M+1;
			end
			D = ValDate - normal_year(M);
		end
		
		if (lhs==1) then
			Y=[Y,M,D,h,m,s];
		end
	
	else
		error('parameter incorrect.');
	end

endfunction
