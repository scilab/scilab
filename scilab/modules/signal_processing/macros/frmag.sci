// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [xm,fr]=frmag(num,den,npts)
    //[xm,fr]=frmag(num[,den],npts)
    //Calculates the magnitude of the frequency respones of
    //FIR and IIR filters.  The filter description can be
    //one or two vectors of coefficients, one or two polynomials,
    //or a rational polynomial.
    //Case 1 (When den is not given):
    //  num  :Vector coefficients/Polynomial/Rational
    //       :polynomial of filter
    //Case 2 (When den is given):
    //  num  :Vector coefficients/Polynomial of filter numerator
    //  den  :Vector coefficients/Polynomial of filter denominator
    //Case 1 and 2:
    //  npts :Number of points desired in frequency response
    //  xm   :Magnitude of frequency response at the points fr
    //  fr   :Points in the frequency domain where
    //       :magnitude is evaluated
    //!

    select argn(2)
    case 2 then //frmag(sys,npts)
        npts=den;
        if typeof(num)=="rational" then
            h=num;
            if size(h,"*")<>1 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A single input, single output system expected.\n"),"frmag",1))
            end
            num=h.num;den=h.den;
        elseif typeof(num)=="state-space" then
            h=ss2tf(num);
            if size(h,"*")<>1 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A single input, single output system expected.\n"),"frmag",1))
            end
            num=h.num;den=h.den;
        elseif type(num)==2 then,
            if size(num,"*")<>1 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A polynomial expected.\n"),"frmag",1));
            end
            den=poly(1,"z","c");
        elseif type(num)==1 then,
            num=poly(num,"z","c");
            den=1
        else
            error(msprintf(_("%s: Wrong type for input argument #%d:  Linear dynamical system or row vector of floats expected.\n"),"frmag",1))
        end,
    case 3 then,//frmag(num,den,npts)
        if type(num)==2 then,
            if size(num,"*")<>1 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A polynomial expected.\n"),"frmag",1));
            end
        elseif type(num)==1 then,
            num=poly(num,"z","c");
        else,
            error(msprintf(_("%s: Wrong size for input argument #%d: A polynomial expected.\n"),"frmag",1));
        end,
        if type(den)==2 then,
            if size(den,"*")<>1 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A polynomial expected.\n"),"frmag",2));
            end
        elseif type(den)==1 then,
            den=poly(den,"z","c");
        else,
            error(msprintf(_("%s: Wrong size for input argument #%d: A polynomial expected.\n"),"frmag",2));
        end,
    end
    fr=linspace(0,1/2,npts+1);
    fr($)=[];
    dfr=exp(2*%i*%pi*fr);
    xm=abs(freq(num,den,dfr));
endfunction
