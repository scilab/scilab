//------------------------------------------------------------------------
// Allan CORNET
// INRIA 2004
//------------------------------------------------------------------------
// Convert to serial date number
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
//------------------------------------------------------------------------
function scalaire=ymdhmns_to_scalar (annee,mois,jour,heure,mn,sec)
  scalaire = (sec*(1/(24*3600*60)))+(mn*(1/(24*3600)))+(heure*(1/ 24))
  scalaire = scalaire + jour + months_to_days(mois);
  if (mois > 2) then
    if ( IsBissextile(annee) == %T ) then
      scalaire = scalaire - 1;
    else
      scalaire = scalaire - 2;
    end
    annee=annee;
    scalaire = scalaire+years_to_days(annee);
  end
endfunction
//------------------------------------------------------------------------
function Rep=IsBissextile(annee)
  Rep=%F
  r0 = modulo(annee,400);
  r1 = modulo(annee,4);
  r2 = modulo(annee,100);
  if ( (r0 == 0 ) | ((r1 ==  0) & (r2 == 0)) ) then Rep=%T,end;
endfunction
//------------------------------------------------------------------------
function days=months_to_days (mois)
  days=(mois * 3057 - 3007) / 100;
endfunction
//------------------------------------------------------------------------
function days=years_to_days (annee)
  days=annee * 365 + (annee / 4) - (annee / 100) + (annee / 400);
endfunction
//------------------------------------------------------------------------
