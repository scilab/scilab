// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function %g=numdiff(%__func,%x,%dx)
    warnobsolete("numderivative","6.0")
    // given a function %__func from R^n to R^p
    //computes the matrix g such as
    //    [   d f    ]
    //    [      i   ]
    //g = [   ----   ]
    // ij [   d x    ]
    //    [      j   ]
    // using finite difference methods
    if type(%__func)==15 then
        params=%__func;params(1)=null();
        %__func=%__func(1)
    else
        params=list()
    end
    if type(%__func)==10 then //hard coded function given by its name
        error(msprintf(gettext("%s: Hard coded function not allowed, create a Scilab function using call."),"numdiff"));
    end

    %x=%x(:);
    %n=size(%x,"*")
    if argn(2)<3 then
        %dx=sqrt(%eps)*(1+1d-3*abs(%x))
    end
    if size(params)==0 then
        %y0=%__func(%x)
        %g(size(%y0,1),%n)=0
        for %j=1:%n
            %v=0*%x;%v(%j)=%dx(%j);
            %g(:,%j)=(-%y0+%__func(%x+%v))/%dx(%j);
        end
    else
        %y0=%__func(%x,params(:))
        %g(size(%y0,1),%n)=0
        for %j=1:%n
            %v=0*%x;%v(%j)=%dx(%j);
            %g(:,%j)=(-%y0+%__func(%x+%v,params(:)))/%dx(%j);
        end
    end
endfunction
