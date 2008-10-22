//------------------------------------------------------------------------
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Pierre MARECHAL
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// Return day of week
// Maurice Kraitchik Algorithm
//------------------------------------------------------------------------

function [N,S] = weekday(D,form)
	
	lhs=argn(1);
	rhs=argn(2);
	
	// checkings
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"),"weekday",1,2));
	end
	
	if type(D) <> 1 then
		error(msprintf(gettext("%s: Wrong type  for input argument #%d: Real constant matrix expected.\n"),"weekday",1));
	end
	
	if (rhs==2) & (form <> 'long') & (form <> 'short') then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"),"weekday",2,"long","short"));
	end
	
	// some definitions
	
	common_year  = [0,31,59,90,120,151,181,212,243,273,304,334,365];
	leap_year    = [0,31,60,91,121,152,182,213,244,274,305,335,366];
	week_numbers = [7,1,2,3,4,5,6];
	
	week_strings_short = [gettext("Sat"), ..
		gettext("Sun"), ..
		gettext("Mon"), ..
		gettext("Tue"), ..
		gettext("Wed"), ..
		gettext("Thu"), ..
		gettext("Fri")];
	
	week_strings_long = [gettext("Saturday"), ..
		gettext("Sunday")    , ..
		gettext("Monday")    , ..
		gettext("Tuesday")   , ..
		gettext("Wednesday") , ..
		gettext("Thursday")  , ..
		gettext("Friday")    , ..
		gettext("Jan")];
	
	
	if rhs==2 then
		if form == 'long' then
			week_strings = week_strings_long;
		else
			week_strings = week_strings_short;
		end
	else
		week_strings = week_strings_short;
	end
	
	
	// Algorithm
	
	[nr,nc]  = size(D);
	
	if nc == 1 then
		common_year = common_year';
		leap_year   = leap_year';
	end
	
	month_day_mat   = ones(nr,nc);
	
	m               = ones(nr,nc);
	y               = ones(nr,nc);
	d               = ones(nr,nc);
	
	Y        = floor(D/365.2425);
	temp     = D - (365.0*Y + ceil(0.25*Y)- ceil(0.01*Y) + ceil(0.0025*Y));
	
	mask     = (temp <= 0);
	Y(mask)  = Y(mask) - 1;
	D(mask)  = D(mask) - (365.0*Y(mask) + ceil(0.25*Y(mask)) - ceil(0.01*Y(mask)) + ceil(0.0025*Y(mask)));
	D(~mask) = temp(~mask)
	
	M = int(D/29);
	
	month_day_mat(isLeapYear(Y))  = leap_year(M(isLeapYear(Y))+1);
	month_day_mat(~isLeapYear(Y)) = common_year(M(~isLeapYear(Y))+1);
	
	M( D>month_day_mat ) = M( D>month_day_mat )+1;
	
	month_day_mat(isLeapYear(Y))  = leap_year(M(isLeapYear(Y)));
	month_day_mat(~isLeapYear(Y)) = common_year(M(~isLeapYear(Y)));
	
	d = D - month_day_mat;
	
	m(M==1) = 13;
	m(M==2) = 14;
	
	y( M==1 | M==2 ) = Y( M==1 | M==2 ) - 1;
	
	m( M<>1 & M<>2 ) = M( M<>1 & M<>2 );
	y( M<>1 & M<>2 ) = Y( M<>1 & M<>2 );
	
	n =  modulo(  (d + floor(2*m) + floor(3*(m+1)/5) + y + floor(y/4) - floor(y/100) + floor(y/400) + 2)  ,  7  );
	
	N = matrix( week_numbers(n+1) , nr, nc );
	S = matrix( week_strings(n+1) , nr, nc );
	
endfunction
