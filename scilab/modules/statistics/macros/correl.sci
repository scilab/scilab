// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//

function rho=correl(varargin)
    //
    //This function computes  the correlation of two variables x
    //and y where x is  a numerical vector of length  n, y is  a
    //numerical  vector  of length  m  and fre   is an array  of
    //dimensions nxm. In    fre  the element  of   indices (i,j)
    //corresponds to the number or frequencies of x_i&y_j.
    //
    //References: Wonacott,  T.H. & Wonacott, R.J.; Introductory Statistics,
    //J.Wiley & Sons, 1990.
    //
    //
    rhs = argn(2);
    if rhs < 2 | rhs > 3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"correl",2,3));
    end

    x = varargin(1);
    y = varargin(2);

    if type(x) <> 1 | ~isvector(x) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Vector expected.\n"),"correl",1));
    end

    if type(y) <> 1 | ~isvector(y) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Vector expected.\n"),"correl",2));
    end

    if isempty(x) | isempty(y) then
        rho = %nan
        return;
    end

    cx = size(x, "*");
    ly = size(y, "*");
    x=matrix(x,1,cx);
    y=matrix(y,ly,1);

    if rhs == 2 then
        if cx <> ly then
            error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected"), "correl", 1, 2));
        end
        mx = mean(x);
        my = mean(y);
        sx = sqrt(sum((x-mx).^2))
        sy = sqrt(sum((y-my).^2))
        rho = (x-mx)*(y-my) / (sx*sy);
    else
        fre = varargin(3);
        [lfre, cfre] = size(fre);

        if cx <> lfre then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Same number of line as first input argument expected.\n"),"correl",3));
        end
        if ly <>cfre then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Same number of column as first input argument expected.\n"),"correl",3));
        end

        fr=fre/sum(fre)
        px=sum(fr,"c")
        py=sum(fr,"r")
        mx = x * px;
        my = py*y;
        sx = sqrt(((x-mx).^2)*px);
        sy = sqrt(py*((y-my).^2));
        sxy = sum((x-mx)*fr*(y-my));
        rho=sxy/(sx*sy)
    end

endfunction
