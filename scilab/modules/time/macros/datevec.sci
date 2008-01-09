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
	
	common_year = [0,31,59,90,120,151,181,212,243,273,304,334,365];
	leap_year   = [0,31,60,91,121,152,182,213,244,274,305,335,366];
	
	if rhs<>1 then
		error(gettext('parameter incorrect.'));
	end
	
	if type(N) <> 1 then
		error(gettext('The argument must a real constant matrix.'));
	end
	
	[nr,nc] = size(N);
	compteur = 0;
	
	// for the moment : hour, minute, second
	// =========================================================================
	
	second = 86400*(N-floor(N));
	hour   = floor(second/3600);
	second = second - 3600*hour;
	minute = floor(second/60);
	second = second - 60*minute;
	
	// Now, the year
	// =========================================================================
	
	N    = floor(N);
	Year = floor(N/365.2425);
	
	temp = N - (365.0*Year + ceil(0.25*Year)- ceil(0.01*Year) + ceil(0.0025*Year));
	
	// On retranche 1 si la valeur est inferieur à 0
	
	mask       = (temp <= 0);
	Year(mask) = Year(mask)-1;
	
	N(mask)    = N(mask) - (365.0*Year(mask) + ceil(0.25*Year(mask)) - ceil(0.01*Year(mask)) + ceil(0.0025*Year(mask)));
	N(~mask)   = temp(~mask);
	
	// ... and the month
	// =========================================================================
	
	Month = int (N/29);
	
	// construction de la matrice
	
	month_day_mat = ones(nr,nc);
	
	month_day_mat(isLeapYear(Year))  = leap_year(Month(isLeapYear(Year))+1);
	month_day_mat(~isLeapYear(Year)) = common_year(Month(~isLeapYear(Year))+1);
	
	Month( N>month_day_mat ) = Month( N>month_day_mat )+1;
	
	Day = ones(nr,nc);
	
	Day(isLeapYear(Year))  = N(isLeapYear(Year))  - leap_year(Month(isLeapYear(Year)))';
	Day(~isLeapYear(Year)) = N(~isLeapYear(Year)) - common_year(Month(~isLeapYear(Year)))';
	
	
	if (lhs==1) then
		Y(:,1) = matrix(Year  ,nr*nc , 1);
		Y(:,2) = matrix(Month ,nr*nc , 1);
		Y(:,3) = matrix(Day   ,nr*nc , 1);
		Y(:,4) = matrix(hour  ,nr*nc , 1);
		Y(:,5) = matrix(minute,nr*nc , 1);
		Y(:,6) = matrix(second,nr*nc , 1);
	else
		Y = Year;
		M = Month;
		D = Day;
		h = hour;
		m = minute;
		s = second;
	end
	
endfunction
