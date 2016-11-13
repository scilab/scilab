
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

function [m]=meanf(val,fre,orient)
    //
    //This function returns in scalar m the  mean of the  values of a vector
    //or matrix   val,  each  counted  with a  frequency   signaled  by  the
    //corresponding values of the integer vector or matrix fre.
    //
    //For  a vector or matrix  val, m=meanf(val,fre) or m=meanf(val,fre,'*')
    //returns in scalar m the  mean of all the  entries  of val, each  value
    //counted with the multiplicity indicated  by the corresponding value of
    //fre.
    //
    //m=meanf(val,fre,'r')(or, equivalently, m=meanf(val,fre,1))  returns in
    //each entry of  the row vector m  of type  1xsize(val,'c') the mean  of
    //each column of val, each value counted with the multiplicity indicated
    //by the corresponding value of fre.
    //
    //m=meanf(val,fre,'c')(or, equivalently, m=meanf(val,fre,2)) returns  in
    //each entry of the column vector m of  type size(val,'c')x1 the mean of
    //each row of val, each value counted with the multiplicity indicated by
    //the corresponding value of fre.
    //
    //References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
    //Statistics, J.Wiley & Sons, 1990.
    //
    [lhs, rhs] = argn(0);

    if rhs == 0 | rhs == 1| rhs >= 4 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"meanf",2,3));
    end

    // If val is not constant, sparse, integer, hypermat
    if and(type(val) <> [1 5 8]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A full or sparse matrix, or an integer matrix, or an hypermat expected.\n"),"meanf",1));
    end

    // If fre is not constant, sparse, integer, hypermat
    if and(type(fre) <> [1 5 8]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A full or sparse matrix, or an integer matrix, or an hypermat expected.\n"),"meanf", 2));
    end

    if or(size(val) <> size(fre)) && (size(val, "*") <> 1 && size(fre, "*") <> 1 && ~isempty(fre)) then
        error(msprintf(gettext("%s: Wrong size for input arguments #%d and #%d: Same dimensions expected.\n"), "meanf", 1, 2));
    end

    if val == [] | fre == [] | and(fre == 0) then
        m = %nan;
        return
    end

    if rhs == 2 then
        m=sum(val .* fre)/sum(fre);
    elseif rhs==3 then
        if orient=="*" then
            m=sum(val .* fre)/sum(fre)
        elseif orient=="r"|orient==1 then
            m=sum(val .* fre,"r") ./ sum(fre,"r")
        elseif orient=="c"|orient==2 then
            m=sum(val .* fre,"c") ./ sum(fre,"c")
        else
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', %d or %d expected.\n"),"meanf",3,"r","c",1,2)),
        end
    end
endfunction
