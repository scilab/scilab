// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
// Copyrifht (C) 2012 - Scilab Enterprises - Adeline CARNIS
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function A = %sp_gsort(A, optsort, directionsort)
    [ij, v, mn] = spget(A);

    if mn(1) <> 1 & mn(2) <> 1 then
        error(999,msprintf(_("%s: Wrong size for input argument #%d: sparse vectors expected.\n"),'gsort',1));
    end

    if (strcmp(optsort, 'c')) == 0 | v == [] then
        A = A;
    else
        if mn(2) == 1 then
            dif = mn(1) - length(v);
            v = gsort(v, optsort, directionsort);
        elseif mn(1) == 1 then
            dif = mn(2) - length(v);
            v = gsort(v, optsort, directionsort);
        end

        last = find(v<0);
        first = find(v>0);

        if last == [] & first <> [] then
            if strcmp(directionsort, 'i')== 0 then
                ij(:,1) = first(:) + dif;
            else
                ij(:,1) = first(:);
            end
        elseif first == [] & last <> [] then
            ij(:,1) = last(:) + dif;
        else
            if strcmp(directionsort, 'i')== 0 then
                ij(:,1) = [last(:); first(:) + dif];
            else
                ij(:,1) = [first(:); last(:) + dif];
            end
        end

        A = sparse(ij, v, mn)
    end
endfunction

