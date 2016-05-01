// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// Copyright (C) 2012 - SCilab Enterprises - Cedric Delamarre
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y=asciimat(x)
    // This function converts a matrix of string into a matrix of ascii codes using ascii() Scilab function
    // and converts an array of ascii codes into a array of string
    // Returned value have same size as input value instead of second dims !
    // Fonction created because ascii() Scilab function returns a row vector

    if x==[] then
        y=[]
        return
    end

    dims = size(x);
    if typeof(x) == "string" // convert string to ascii code
        if size(dims,"*") > 2 // hypermatrix case
            colref = 0;
            lastDim = dims($);
            dims($) = [];
            l=list();
            for i=1:size(dims,"*")
                l(i) = 1:$;
            end
            for i=1:lastDim
                res=asciimat(x(l(:), i));
                if colref == 0 then
                    colref=size(res,"c");
                else
                    if colref <> size(res,"c")
                        error(msprintf(gettext("%s: Wrong input argument #%d: Inconsistent size.\n"),"asciimat", 1));
                        return
                    end
                end
                y(l(:), i) = res;
            end
        else // 2D matrix case | ["a" "bc";"de" "f"] => [97 98 99;100 101 102]
            x=x';
            a = ascii(x(:));
            aSize = size(a, "*");
            dims(2) = 1;
            p = prod(dims);
            if modulo(aSize, p)
                error(msprintf(gettext("%s: Wrong input argument #%d: Inconsistent size.\n"),"asciimat", 1));
            end
            dims(2) = dims(1);
            dims(1) = aSize/p;
            y = matrix(a, dims)';
        end
    else    // convert asciicode to string
        if size(dims,"*") > 2 // hypermatrix case
            lastDim = dims($);
            dims($) = [];
            l=list();
            for i=1:size(dims,"*")
                l(i) = 1:$;
            end
            for i=1:lastDim
                y(l(1:$-1), i) = asciimat(x(l(:), i))
            end
        else // 2D matrix case | [97 98 99;100 101 102] => ["abc";"def"]
            y = ascii(x');
            // a is a scalar string
            if modulo(length(y), dims(2))
                error(msprintf(gettext("%s: Wrong input argument #%d: Inconsistent size.\n"),"asciimat", 1));
            end
            if dims(1) <> 1 && dims(2) <> 1 then
                y=strsplit(y, cumsum(dims(2) * ones(1,dims(1)-1)));
            end
        end
    end
endfunction
