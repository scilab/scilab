
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1999 - INRIA - Carlos Klimann
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function [q]=iqr(x,orien)
    //
    //This  function computes the interquartile  range IQR= upper quartile -
    //lower quartile of a vector or a matrix x.
    //
    //For  a  vector or  a matrix  x, q=iqr(x) returns  in  the scalar q the
    //interquartile range of all the entries of x.
    //
    //q=iqr(x,'r')    (or,   equivalently,  q=iqr(x,1))    is   the  rowwise
    //interquartile range. It returns in each entry of the  row vector q the
    //interquartile range of each column of x.
    //
    //q-iqr(x,'c')    (or,  equivalently, q=iqr(x,2))    is  the  columnwise
    //interquartile range. It returns in  each entry of  the column vector q
    //the interquartile range of each row of x.
    //
    //References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
    //Statistics, J.Wiley & Sons, 1990.
    //
    //
    [lhs,rhs] = argn(0);

    if rhs == 0 then
        error(msprintf(gettext("%s: Wrong number of input argument: At least %d expected.\n"),"iqr",1));
    end

    if rhs == 1 then
        if x == [] | and(isnan(x)) then
            q = %nan;
            return
        end

        qq = quart(x)
        q = qq(3) - qq(1);
    else
        qq=quart(x,orien);
        if orien == "r" | orien == 1 then
            if size(x,1) == 1 then
                error(msprintf(_("%s: Wrong dimensions for input argument #%d: A column vector or matrix expected.\n"), "iqr", 1));
            end
            q = qq(3, :) - qq(1, :);
        elseif orien == "c" | orien == 2 then
            if size(x,2) == 1 then
                error(msprintf(_("%s: Wrong dimensions for input argument #%d: A row vector or matrix expected.\n"), "iqr", 1));
            end
            q = qq(:,3) - qq(:, 1);
        else
            error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'', %d, ''%s'' or %d expected.\n"), "iqr", 2, "r", 1, "c", 2));
        end
    end
endfunction
