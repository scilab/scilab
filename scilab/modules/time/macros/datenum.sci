//------------------------------------------------------------------------
// Allan CORNET
// INRIA 2004
//
// Modified by Pierre MARECHAL
// Scilab team
// Copyright INRIA
// Date : 28 Dec 2005
//
// Convert to serial date number
//------------------------------------------------------------------------

function n=datenum(varargin)

	lhs=argn(1);
	rhs=argn(2);
	
	select rhs
		case 0
			Date = getdate();
			
			n = ymdhmns_to_scalar(Date(1),..
				Date(2),..
				Date(6),..
				Date(7),..
				Date(8),..
				Date(9));
			
			break
			
		case 1
			
			DateIn = varargin(1);
			
			// Checks
			
			if type(DateIn)<> 1 then
				error(gettext('Type of parameters incorrect.'));
			end
			
			[nr,nc] = size(DateIn);
			
			if (nc <> 3) & (nc <> 6) then
				error(gettext('Argument must be a m*3 matrix or a m*6 matrix.'));
			end
			
			if min(DateIn(:,2))<1 | max(DateIn(:,2))>12 then
				error(gettext('Month argument must be between 1 and 12.'));
			end
			
			if min(DateIn(:,3))<1 | max(DateIn(:,3))>31 then
				error(gettext('Day argument must be between 1 and 31.'));
			end
			
			if nc == 6 then
				
				if min(DateIn(:,4))<0 | max(DateIn(:,4))>23 then
					error(gettext('Hour argument must be between 0 and 23.'));
				end
				
				if min(DateIn(:,5))<0 | max(DateIn(:,5))>59 then
					error(gettext('Minute argument must be between 0 and 59.'));
				end
				
				if min(DateIn(:,6))<0 | max(DateIn(:,6))>59 then
					error(gettext('Second argument must be between 0 and 59.'));
				end
				
			end
			
			if nc == 3 then
				n = ymdhmns_to_scalar(DateIn(:,1),DateIn(:,2),DateIn(:,3),zeros(nr),zeros(nr),zeros(nr));
			else
				n = ymdhmns_to_scalar(DateIn(:,1),DateIn(:,2),DateIn(:,3),DateIn(:,4),DateIn(:,5),DateIn(:,6));
			end
			break
			
		case 3
			
			YearIn  = varargin(1);
			MonthIn = varargin(2);
			DayIn   = varargin(3);
			
			// checks
			
			if (type(YearIn)<> 1) | (type(MonthIn)<> 1) | (type(DayIn)<> 1) then
				error(gettext('Type of parameters incorrect.'));
			end
			
			if (size(YearIn) <> size(MonthIn)) | (size(YearIn) <> size(DayIn)) then
				error(gettext('The three arguments must have the same size.'));
			end
			
			if min(MonthIn)<1 | max(MonthIn)>12 then
				error(gettext('The second argument must be between 1 and 12.'));
			end
			
			if min(DayIn)<1 | max(DayIn)>31 then
				error(gettext('The third argument must be between 1 and 31.'));
			end
			
			[nr,nc]  = size(YearIn);
			n        = ymdhmns_to_scalar(YearIn,MonthIn,DayIn,zeros(nr,nc),zeros(nr,nc),zeros(nr,nc));
			
			break
		
		case 6
			
			YearIn  = varargin(1);
			MonthIn = varargin(2);
			DayIn   = varargin(3);
			HourIn  = varargin(4);
			MinIn   = varargin(5);
			SecIn   = varargin(6);
			
			// checks
			
			if  (type(YearIn) <> 1) | ..
				(type(MonthIn)<> 1) | ..
				(type(DayIn)  <> 1) | ..
				(type(HourIn) <> 1) | ..
				(type(MinIn)  <> 1) | ..
				(type(SecIn)  <> 1) then
				error(gettext("Type of parameters incorrect."));
			end
			
			if  (size(YearIn) <> size(MonthIn)) | ..
				(size(YearIn) <> size(DayIn))   | ..
				(size(YearIn) <> size(HourIn))  | ..
				(size(YearIn) <> size(MinIn))   | ..
				(size(YearIn) <> size(SecIn))  then
				error(gettext("The six arguments must have the same size."));
			end
			
			if min(MonthIn)<1 | max(MonthIn)>12 then
				error(gettext("The second argument must be between 1 and 12."));
			end
			
			if min(DayIn)<1 | max(DayIn)>31 then
				error(gettext("The third argument must be between 1 and 31."));
			end
			
			if min(HourIn)<0 | max(HourIn)>23 then
				error(gettext("The fourth argument must be between 0 and 23."));
			end
			
			if min(MinIn)<0 | max(MinIn)>59 then
				error(gettext("The fifth argument must be between 0 and 59."));
			end
			
			if min(SecIn)<0 | max(SecIn)>59 then
				error(gettext("The sixth argument must be between 0 and 59."));
			end
			
			n = ymdhmns_to_scalar(YearIn,MonthIn,DayIn,HourIn,MinIn,SecIn);
			
			break
		else
			error(gettext('Number of parameters incorrect.'));
	end
	

endfunction


function scalaire=ymdhmns_to_scalar (annee,mois,jour,heure,mn,sec)
	
	decimal_part = (sec*(1/(24*3600)))+(mn*(1/(24*60)))+(heure*(1/24));
	
	// Concersion des mois et jour
	integer_part = jour + floor((mois * 3057 - 3007) / 100);
	
	// On retranche 1 si le mois est au dela de février
	integer_part = integer_part + ((mois < 3) - 1);
	
	// On retranche encore 1 si le mois est au dela de février et année non bissextile
	integer_part = integer_part + (((mois < 3)|(isLeapYear(annee))) -1);
	
	// Convertion des année
	leap_year_case     = annee * 365 + (annee / 4) - floor(annee / 100) + floor(annee / 400);
	not_leap_year_case = annee * 365 + floor(annee/4) + 1 - floor(annee / 100) + floor(annee / 400);
	
	leap_year_case(~isLeapYear(annee))    = 0;
	not_leap_year_case(isLeapYear(annee)) = 0;
	
	integer_part       = integer_part + leap_year_case + not_leap_year_case;
	
	scalaire = integer_part+decimal_part;
	
endfunction
