// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
// Copyrifht (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [A, k] = %sp_gsort(A, optsort, directionsort)
    rhs = argn(2);
    lhs = argn(1);
    // arguments by default in gsort
    select rhs
    case 1
        optsort = "g";
        directionsort = "d";
    case 2
        // optsort can be: 'r', 'c', 'g', 'lr', 'lc'
        pos_opts = find(optsort == ["r", "c", "g", "lr", "lc"]);
        if pos_opts == [] then
            error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"), "gsort", 2, "r", "c", "g", "lr", "lc"));
        end
        directionsort = "d";
    else
        // optsort can be: 'r', 'c', 'g', 'lr', 'lc'
        pos_opts = find(optsort == ["r", "c", "g", "lr", "lc"]);
        // directionsort can be: 'd' or 'i'
        pos_direction = find(directionsort == ["d", "i"]);
        if pos_opts == [] then
            error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"), "gsort", 2, "r", "c", "g", "lr", "lc"));
        end
        if pos_direction == [] then
            error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"), "gsort", 3, "d", "i"));
        end
    end

    [ij, v, mn] = spget(A);
    if mn(1) <> 1 & mn(2) <> 1 then
        error(999,msprintf(_("%s: Wrong size for input argument #%d: sparse vectors expected.\n"), "gsort", 1));
    end

    if mn(1) == 1 then
        // if A is a row vector and optsort = 'r', the result is the
        // first input argument
        if strcmp(optsort, "r") == 0 |strcmp(optsort, "lr") == 0 | v == [] then
            A = A;
            if lhs == 2 then
                if strcmp(optsort, "lr") == 0 | ij == [] then
                    k = 1;
                else
                    k = ij(:,1);
                    k = k';
                end
            end
        else
            dif = mn(2) - length(v);
            if lhs == 1 then
                v = gsort(v', optsort, directionsort);
            else
                [v, k] = gsort(v', optsort, directionsort);
                k=ij(k,2)';
            end

            //Obtain the indices corresponding to positive values of v
            // and negative value of v
            // If A is complex, the elements are sorted by magnitude
            if isreal(A) then
                last = find(v<0);
                first = find(v>0);
            else
                s = abs(v);
                last = find(s<0);
                first = find(s>0);
            end

            // Sort the indices
            if last == [] & first <> [] then
                if strcmp(directionsort, "i")== 0 then
                    ij(:,2) = first(:) + dif;
                else
                    ij(:,2) = first(:);
                end
            elseif first == [] & last <> [] then
                if strcmp(directionsort, "i")== 0 then
                    ij(:,1) = last(:);
                else
                    ij(:,1) = last(:) + dif;
                end
            else
                if strcmp(directionsort, "i")== 0 then
                    ij(:,2) = [last(:); first(:) + dif];
                else
                    ij(:,2) = [first(:); last(:) + dif];
                end
            end
            A = sparse(ij,v,mn);
        end
    end

    if mn(2) == 1 then
        // if A is a column vector and optsort = 'c', the result is the
        // first input argument
        if strcmp(optsort, "c") == 0 | strcmp(optsort, "lc") == 0 | v == [] then
            A = A;
            if lhs == 2 then
                if strcmp(optsort, "lc") == 0 | ij == [] then
                    k = 1;
                else
                    k = ij(:,2);
                    k = k;
                end
            end
        else

            dif = mn(1) - length(v);
            if lhs == 1 then
                v = gsort(v, optsort, directionsort);
            else
                [v, k] = gsort(v, optsort, directionsort);
                k=ij(k,1);
            end

            //Obtain the indices corresponding to positive values of v
            // and negative value of v
            // If A is complex, the elements are sorted by magnitude
            if isreal(A) then
                last = find(v<0);
                first = find(v>0);
            else
                s = abs(v);
                last = find(s<0);
                first = find(s>0);
            end

            // sort the indices in terms of directionsort = 'i' or 'd'
            // if directionsort='i' and v>0, the elements are sorted in the
            // increasing order, ie [0,..,v] and, conversely, in the decreasing
            // order the elements are sorted : [v,..,0]
            // if v<0, the elements are sorted in the increasing order,
            // ie [v,..,0] and, conversely, in the decreasing order the
            // elements are sorted : [0,..,v]
            // And else, if v contains positive and neqative values, the
            // elements are sorted in the increasing order,ie [v_neg,0,v_pos],
            // and conversely for the decreasing order.
            if last == [] & first <> [] then
                if strcmp(directionsort, "i") == 0 then
                    ij(:,1) = first(:) + dif;
                else
                    ij(:,1) = first(:);
                end
            elseif first == [] & last <> [] then
                if strcmp(directionsort, "i") == 0 then
                    ij(:,1) = last(:);
                else
                    ij(:,1) = last(:) + dif;
                end
            else
                if strcmp(directionsort, "i") == 0 then
                    ij(:,1) = [last(:); first(:) + dif];
                else
                    ij(:,1) = [first(:); last(:) + dif];
                end
            end
            A = sparse(ij, v, mn);
        end
    end
endfunction
