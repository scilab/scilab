// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2012 - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y = nthroot(x,n)
    
    rhs = argn(2);
    
    // if the number of input arguments is wrong
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "nthroot", 2));
    end
    
    // if x or n are not real
    if ((typeof(x) <> "constant" | ~isreal(x)) | (typeof(n) <> "constant" | ~isreal(n))) then
        error(msprintf(gettext("%s: Wrong type for input argument(s) %d: real arguments expected.\n"),"nthroot",2));
    end
    
    // if n is not scalar
    if (size(n,"*")~=1) then
        error(msprintf(gettext("%s: Wrong type for input argument(s) %d: n must be a scalar.\n"),"nthroot",1));
    end
        
    reste = modulo(n,2);
    y = x;
    
    // if n = %nan and x is positive then y = %nan
    if (isnan(n) & or(x >= 0)) then
        y(find(x>=0)) = %nan;
    elseif (x==[]) then
        y = [];
    elseif (n==0 & (x>=0 | isnan(x))) then
        // if n = 0 and x = 1 or x = %nan then y = %nan
        y(find((x==1 | isnan(x)))) = %nan;
        // if n = 0 and x>1 then y = %inf
        y(find(x>1)) = %inf;
        // if n = 0 and x = %eps then y = 0
        y(find(x==%eps)) = 0;
    elseif (or (or(x(:)<0) & (n~=fix(n) | reste ==0))) then
        error(msprintf(gettext("%s: If x is negative, n must be an odd integer\n"),"nthroot"));
        // if n ~=0 and n ~= %nan
    elseif (n~=0 & ~isnan(n)) then
        //if x = 0 and n is negative and n i~= %nan
        [m1,m2] = size(x(find(x==0 & n<0 & ~isinf(n))));
        y(find(x==0 & n<0 & ~isinf(n))) = (x(find(x==0 & n<0 & ~isinf(n)))+ones(m1,m2)) .*%inf;
        //if x = 0 and n is positive and n ~= %nan
        y(find(x==0 & (n>0 |isinf(n)))) = x(find(x==0 & (n>0 |isinf(n)))).^(1 ./n);
        // if x is positive
        y(find(x>0)) = x(find(x>0)).^(1 ./n);
        // if x is negative
        y(find(x<0)) = sign(x(find(x<0))).*(abs(x(find(x<0)))).^(1 ./n);
    end    
    
     
endfunction



    