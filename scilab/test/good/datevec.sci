//------------------------------------------------------------------------
// Allan CORNET
// INRIA 2004
//
// Modified by Pierre MARECHAL
// Scilab team
// Copyright INRIA
// Date : 28 Dec 2005
//------------------------------------------------------------------------

function [Y,M,D,h,m,s] = datevec(N)

	lhs=argn(1);
	rhs=argn(2);
	
	normal_year = [0,31,59,90,120,151,181,212,243,273,304,334,365];
	bissextile_year = [0,31,60,91,121,152,182,213,244,274,305,335,366];
	
	if rhs<>1 then
		error("parameter incorrect.");
	end
	
	if type(N) <> 1 then
		error("The argument must a real constant matrix");
	end
	
	[nr,nc] = size(N);
	compteur = 0;
	
	for j=1:nc
		for i=1:nr
		
			ValDate=N(i,j);
			compteur = compteur + 1;
			
			// for the moment : hour, minute, second
			
			if ValDate == floor(ValDate) then
				hour = 0;
				minute = 0;
				second = 0;
			else
				second = 86400*(ValDate-floor(ValDate));
				hour = floor(second/3600);
				second = second - 3600*hour;
				minute = floor(second/60);
				second = second - 60*minute;
			end
			
			// Now, the year
			
			ValDate = floor(ValDate);
			Year = floor(ValDate/365.2425);
			
			temp = ValDate - (365.0*Year + ceil(0.25*Year)- ceil(0.01*Year) + ceil(0.0025*Year));
			
			if  temp <= 0 then
				Year = Year - 1;
				ValDate = ValDate - (365.0*Year + ceil(0.25*Year) - ceil(0.01*Year) + ceil(0.0025*Year));
			else
				ValDate = temp;
			end
			
			// ... and the month
			
			Month = int (ValDate/29);
			
			if isLeapYear(Year) then
				if ValDate > bissextile_year(Month+1) then
					Month = Month+1;
				end
				Day = ValDate - bissextile_year(Month);
			else
				if ValDate > normal_year(Month+1) then
					Month = Month+1;
				end
				Day = ValDate - normal_year(Month);
			end
			
			if (lhs==1) then
				Y(compteur,1) = Year;
				Y(compteur,2) = Month;
				Y(compteur,3) = Day;
				Y(compteur,4) = hour;
				Y(compteur,5) = minute;
				Y(compteur,6) = second;
			else
				Y(compteur) = Year;
				M(compteur) = Month;
				D(compteur) = Day;
				h(compteur) = hour;
				m(compteur) = minute;
				s(compteur) = second;
			end
		end
	end
	
endfunction
