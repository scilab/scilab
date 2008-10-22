//------------------------------------------------------------------------
// Author : Pierre MARECHAL
// Scilab team
// Copyright INRIA
// Date : 29 Dec 2005
//
// Return day of week
// Maurice Kraitchik Algorithm
//------------------------------------------------------------------------

function [N,S] = weekday(D,form)
	
	lhs=argn(1);
	rhs=argn(2);
	
	if rhs < 1 | rhs > 2 then
		error("Number of parameters incorrect.");
	end
	
	if type(D) <> 1 then
		error("The first argument must a real constant matrix");
	end
	
	if (rhs==2) & (form <> 'long') & (form <> 'short') then
		error("The second parameter must be the string ''long'' or ''short''.");
	end
	
	normal_year = [0,31,59,90,120,151,181,212,243,273,304,334,365];
	bissextile_year = [0,31,60,91,121,152,182,213,244,274,305,335,366];
	week_numbers = [7,1,2,3,4,5,6];
	
	week_strings_short = ['Sat','Sun','Mon','Tue','Wed','Thu','Fri'];
	week_strings_long = ['Saturday','Sunday','Monday','Tuesday','Wednesday','Thursday','Friday'];
	
	if rhs==2 then
		if form == 'long' then
			week_strings = week_strings_long;
		else
			week_strings = week_strings_short;
		end
	else
		week_strings = week_strings_short;
	end
	
	[nr,nc] = size(D);
	
	for i=1:nr
		for j=1:nc
			
			ValDate=floor(D(i,j));
			Y = floor(ValDate/365.2425);
			temp = ValDate - (365.0*Y + ceil(0.25*Y)- ceil(0.01*Y) + ceil(0.0025*Y));
			
			if  temp <= 0 then
				Y = Y - 1;
				ValDate = ValDate - (365.0*Y + ceil(0.25*Y) - ceil(0.01*Y) + ceil(0.0025*Y));
			else
				ValDate = temp;
			end
			
			M = int (ValDate/29);
			
			if isLeapYear(Y) then
				if ValDate > bissextile_year(M+1) then
					M = M+1;
				end
				d = ValDate - bissextile_year(M);
			else
				if ValDate > normal_year(M+1) then
					M = M+1;
				end
				d = ValDate - normal_year(M);
			end
			
			if M == 1 then
				m = 13;
				y = Y-1;
			elseif M==2 then
				m = 14;
				y = Y-1;
			else
				m = M;
				y=Y;
			end
			
			n =  modulo(  (d + floor(2*m) + floor(3*(m+1)/5) + y + floor(y/4) - floor(y/100) + floor(y/400) + 2)  ,  7  );
			N(i,j) = week_numbers(n+1);
			S(i,j) = week_strings(n+1);
		end
	end
	
endfunction
