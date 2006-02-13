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
	
	Y=0;
	M=0;
	D=0;
	H=0;
	MI=0;
	S=0;
	
	select rhs
		case 0
			Date=getdate();
			Y=Date(1);
			M=Date(2);
			D=Date(6);
			H=Date(7);
			MI=Date(8);
			S=Date(9);
			break
		case 1
			if (type(varargin(1))== 1) & (size(varargin(1)) == [1,6]) then
				Date=varargin(1);
				Y=Date(1);
				M=Date(2);
				D=Date(3);
				H=Date(4);
				MI=Date(5);
				S=Date(6);
			else
				error('Type of parameters incorrect.');
			end
			break
		case 3
			if (type(varargin(1))== 1) & (type(varargin(2))== 1) & (type(varargin(3))== 1) then
				Y=varargin(1);
				M=varargin(2);
				D=varargin(3);
			else
				error('Type of parameters incorrect.');
			end
			break
		case 6
			if (type(varargin(1))== 1) & (type(varargin(2))== 1) & (type(varargin(3))== 1) & (type(varargin(4))== 1) & (type(varargin(5))== 1) & (type(varargin(6))== 1) then
				Y=varargin(1);
				M=varargin(2);
				D=varargin(3);
				H=varargin(4);
				MI=varargin(5);
				S=varargin(6);
			else
				error('Type of parameters incorrect.');
			end
			break
		else
			error('Number of parameters incorrect.');
	end
	
	n=ymdhmns_to_scalar(Y, M, D,H,MI,S);

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
