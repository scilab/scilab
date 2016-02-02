// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [p] = derivat(p)
    //pd=derivat(p)  computes the derivative of the polynomial or rational
    //function marix relative to the dummy variable
    //

    select typeof(p)

    case "constant" then
        p = 0*p; // includes case p == []

    case "polynomial" then
        var = varn(p); // dummy variable
        [m,n]=size(p);
        deg = degree(p);
        degmax = max(deg);

        if (degmax < n*m)
            x = poly(0, var);
            pd = zeros(p) * x;
            for i = 1:degmax
                pd = pd + coeff(p,i) * i * x^(i-1);
            end
            p = pd;
        else
            for i=1:m
                for j=1:n
                    pij=p(i,j);
                    nij=deg(i,j);
                    if (nij==0) then
                        p(i,j)=0;
                    else
                        pij=coeff(pij).*(0:nij);
                        p(i,j)=poly(pij(2:nij+1),var,"c");
                    end
                end
            end
        end

    case "rational" then
        num = p.num;
        den = p.den;

        num = derivat(num) .* den - num .* derivat(den);
        den = den.^2;

        p.num = num;
        p.den = den;

    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A floating point number or polynomial or rational fraction array expected.\n"),"derivat",1))

    end

endfunction
