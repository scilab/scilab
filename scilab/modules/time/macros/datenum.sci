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
			Date =getdate();
			Y = Date(1);
			M = Date(2);
			D = Date(6);
			H = Date(7);
			MI = Date(8);
			S = Date(9);
			nr = 1;
			break
		case 1
			DateIn = varargin(1);
			
			if type(DateIn)<> 1 then
				error('Type of parameters incorrect.');
			end
			
			[nr,nc] = size(DateIn);
			
			if (nc <> 3) & (nc <> 6) then
				error('Argument must be a m*3 matrix or a m*6 matrix');
			end
			
			if nc == 3 then
				for i=1:nr
					
					if DateIn(i,2)<1 | DateIn(i,2)>12 then
						error('Month argument must be between 1 and 12');
					end
					
					if DateIn(i,3)<1 | DateIn(i,3)>31 then
						error('Day argument must be between 1 and 31');
					end
					
					Y(i,1) = DateIn(i,1);
					M(i,1) = DateIn(i,2);
					D(i,1) = DateIn(i,3);
					H(i,1) = 0;
					MI(i,1) = 0;
					S(i,1) = 0;
				end
			else
				for i=1:nr
					if DateIn(i,2)<1 | DateIn(i,2)>12 then
						error('Month argument must be between 1 and 12');
					end
					
					if DateIn(i,3)<1 | DateIn(i,3)>31 then
						error('Day argument must be between 1 and 31');
					end
					
					if DateIn(i,4)<0 | DateIn(i,4)>23 then
						error('Hour argument must be between 0 and 23');
					end
					
					if DateIn(i,5)<0 | DateIn(i,5)>59 then
						error('Minute argument must be between 0 and 59');
					end
					
					if DateIn(i,6)<0 | DateIn(i,6)>59 then
						error('Second argument must be between 0 and 59');
					end
				
				
					Y(i,1) = DateIn(i,1);
					M(i,1) = DateIn(i,2);
					D(i,1) = DateIn(i,3);
					H(i,1) = DateIn(i,4);
					MI(i,1) = DateIn(i,5);
					S(i,1) = DateIn(i,6);
				end
			end
			break
		case 3
			YearIn = varargin(1);
			MonthIn = varargin(2);
			DayIn = varargin(3);
			
			if (type(YearIn)<> 1) | (type(MonthIn)<> 1) | (type(DayIn)<> 1) then
				error('Type of parameters incorrect.');
			end
			
			if (size(YearIn) <> size(MonthIn)) | (size(YearIn) <> size(DayIn)) then
				error('The three arguments must have the same size.');
			end
			
			[nr,nc] = size(YearIn);
			compteur = 0;
			
			for j=1:nc
				for i=1:nr
					compteur = compteur + 1;
					
					if MonthIn(i,j)<1 | MonthIn(i,j)>12 then
						error('The second argument must be between 1 and 12');
					end
					
					if DayIn(i,j)<1 | DayIn(i,j)>31 then
						error('The third argument must be between 1 and 12');
					end
					
					Y(compteur,1) = YearIn(i,j);
					M(compteur,1) = MonthIn(i,j);
					D(compteur,1) = DayIn(i,j);
					H(compteur,1) = 0;
					MI(compteur,1) = 0;
					S(compteur,1) = 0;
				end
			end
			
			nr = compteur;
			break
		
		case 6
			
			YearIn = varargin(1);
			MonthIn = varargin(2);
			DayIn = varargin(3);
			HourIn = varargin(4);
			MinIn = varargin(5);
			SecIn = varargin(6);
			
			if (type(YearIn)<> 1) | (type(MonthIn)<> 1) | (type(DayIn)<> 1) | (type(HourIn)<> 1) | (type(MinIn)<> 1) | (type(SecIn)<> 1)                 then
				error('Type of parameters incorrect.');
			end
			
			
			if (size(YearIn) <> size(MonthIn)) | (size(YearIn) <> size(DayIn)) | (size(YearIn) <> size(HourIn)) | (size(YearIn) <> size(MinIn)) | (size(YearIn) <> size(SecIn))      then
				error('The six arguments must have the same size.');
			end
			
			[nr,nc] = size(YearIn);
			compteur = 0;
			
			for j=1:nc
				for i=1:nr
				
					compteur = compteur + 1;
					
					if MonthIn(i,j)<1 | MonthIn(i,j)>12 then
						error('The second argument must be between 1 and 12');
					end
					
					if DayIn(i,j)<1 | DayIn(i,j)>31 then
						error('The third argument must be between 1 and 31');
					end
					
					if HourIn(i,j)<0 | HourIn(i,j)>23 then
						error('The fourth argument must be between 0 and 23');
					end
					
					if MinIn(i,j)<0 | MinIn(i,j)>59 then
						error('The fifth argument must be between 0 and 59');
					end
					
					if SecIn(i,j)<0 | SecIn(i,j)>59 then
						error('The sixth argument must be between 0 and 59');
					end
					
					Y(compteur,1) = YearIn(i,j);
					M(compteur,1) = MonthIn(i,j);
					D(compteur,1) = DayIn(i,j);
					H(compteur,1) = HourIn(i,j);
					MI(compteur,1) = MinIn(i,j);
					S(compteur,1) = SecIn(i,j);
				end
			end
			
			nr = compteur;
			break
		else
			error('Number of parameters incorrect.');
	end
	
	for i=1:nr
		n(i) = ymdhmns_to_scalar(Y(i,1),M(i,1),D(i,1),H(i,1),MI(i,1),S(i,1));
	end
	
endfunction


function scalaire=ymdhmns_to_scalar (annee,mois,jour,heure,mn,sec)
	
	decimal_part = (sec*(1/(24*3600)))+(mn*(1/(24*60)))+(heure*(1/24));
	integer_part = jour + months_to_days(mois);
	
	if (mois > 2) then
		if ( isLeapYear(annee) == %T ) then
			integer_part = integer_part - 1;
		else
			integer_part = integer_part - 2;
		end
	end
	
	integer_part = integer_part + years_to_days(annee);
	scalaire = integer_part+decimal_part;
	
endfunction


function days=months_to_days (mois)
	days=floor((mois * 3057 - 3007) / 100);
endfunction


function days=years_to_days (annee)
	
	if ( isLeapYear(annee) == %T ) then
		days=annee * 365 + (annee / 4) - floor(annee / 100) + floor(annee / 400);
	else
		days=annee * 365 + floor(annee/4) + 1 - floor(annee / 100) + floor(annee / 400);
	end
	
endfunction
