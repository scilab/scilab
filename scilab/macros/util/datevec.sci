//------------------------------------------------------------------------
// Allan CORNET
// INRIA 2004
//------------------------------------------------------------------------
function [Y,M,D,h,m,s] = datevec(varargin)

  lhs=argn(1);   
  rhs=argn(2);
  
  if (rhs==1) & (size(varargin(1)) == [1,1]) then
    ValDate=varargin(1);
    Y = 0; 
    h = 0; 
    m = 0;
    s = 0;
    M = ones(size(Y));
    D = ones(size(Y));
    
    // Move day 0 from midnight -0001-12-31 to midnight 0001-3-1
    z = floor(ValDate) - 60; 
    // Calculate number of centuries; K1=0.25 is to avoid rounding problems.
    a = floor((z-0.25)/36524.25);
    // Days within century;  K2=0.25 is to avoid rounding problems.
    b = z - 0.25 + a - floor(a/4);
    // Calculate the year.
    Y = floor(b/365.25);
    // Calculate day in year.
    c = fix(b-floor(365.25*Y)) + 1;
    // Calculate month in year.
    M = fix((5*c + 456)/153);
    D = c - fix((153*M-457)/5);

    // Convert hour-minute-seconds
    s = 86400*(ValDate-floor(ValDate));
    h = floor(s/3600);
    s = s - 3600*h;
    m = floor(s/60);
    s = s - 60*m;
    
    if (lhs==1) then
      Y=[Y,M,D,h,m,s];
    end
    
  else
    error('parameter incorrect.');
  end

endfunction
//------------------------------------------------------------------------