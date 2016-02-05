// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Scilab Enterprises - Adeline CARNIS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [m, k] = %sp_min(varargin)
    [lhs, rhs] = argn(0);

    elements = varargin;
    error_list = %f;

    // If the first argument is a list, it retrieves the number of sparse
    // matrices in list
    if type(varargin(1)) == 15 then
        if rhs <> 1 then
            error(msprintf(_("%s: Wrong size of input argument: %d expected.\n"), "%sp_min", 1))
        end

        rhs = length(varargin(1));
        elements = varargin(1);
        error_list = %t;

        // If the second argument of list is not a sparse -> ERROR
        if rhs == 2 & type(elements(2)) <> 5 then
            error(msprintf(_("%s: Wrong type for input argument #%d: A sparse matrix expected.\n"), "%sp_min", 1))
        end
    end

    select rhs
        // If rhs = 1, the first input argument must be a sparse
    case 1
        A1 = elements(1);

        // Check if A is a sparse
        if type(A1)<> 5 then
            error(msprintf(_("%s: Wrong type for input argument #%d: A sparse matrix expected.\n"), "%sp_min", 1))
        end

        // Retrieves entries of sparse matrix
        [ij, v, mn]= spget(A1);
        if lhs == 1 then
            if v == [] then
                m = 0;
            else
                m = min(v);
            end
        else
            if v == [] then
                m = 0;
                k = [1 1];
            else
                [m, i] = min(v);
                if mn(1) == 1 then
                    k = ij(i,2);
                else
                    if mn(2) == 1 then
                        k = ij(i,1);
                    else
                        k = [ij(i,1) ij(i,2)];
                    end
                end
            end
        end

        // If A1 contains positive values and the length of v is less then mn(1)
        // * mn(2), A1 contains one or several zeros
        // So, min = 0
        if m > 0 & length(v)<> mn(1)*mn(2) then
            m = 0;
            if lhs == 2 then
                k = 0;
                if mn(1) == 1 then
                    for i = 1:length(v)
                        if and(ij(:,2) <> i) then
                            k = i;
                            break;
                        end
                    end
                else
                    if mn(2) == 1 then
                        for i = 1:length(v)
                            if and(ij(:,1) <> i) then
                                k = i;
                                break;
                            end
                        end
                    else
                        // thanks to ij, find the position of zero value
                        // But ij contains only the indices of non zero values
                        // So check on first column the values 1:mn(1)
                        // if posi is less than mn(2) then there is zero value
                        for i = 1:mn(1)
                            posi = length(find(ij(:,1)==i));
                            if posi <> mn(2) then
                                for j = 1:mn(2)
                                    // it is the last index of line where m = 0
                                    if mn(2)*(i-1)+j ==  mn(2)*i then
                                        k = [i, j];
                                        break;
                                    else
                                        pos = find(ij(mn(2)*(i-1)+j,2) == j);
                                        if pos == [] & k == 0 then
                                            k = [i,j];
                                            break;
                                        end
                                    end
                                end
                                if k <> 0 then
                                    break;
                                end
                            end
                        end
                    end
                end
            end
        end

        // If rhs = 2, the second input argument can be a character or a sparse
        // matrix
    case 2
        if lhs == 2 then
            error(msprintf(_("%s: Wrong number of output argument: %d expected"), "%sp_min", 1));
        end

        A1 = elements(1);

        // Check if A is a sparse
        if type(A1) <> 5  then
            error(msprintf(_("%s: Wrong type for input argument #%d: A sparse matrix expected.\n"), "%sp_min", 1));
        end

        select type(elements(2))
            // If the second argument is a string
        case 10
            opts = elements(2);
            // Opts can be : 'c' 'r' or 'm'
            ind = find(opts == ["c","r","m"]);
            if (ind == []) then
                error(msprintf(_("%s: Wrong value for input argument #%d: [''r'' ''c'' ''m''] expected.\n"),"%sp_min",2));
            end

            [ij, v, mn] = spget(A1);

            // If mn(1) = 1, A1 is a row vector
            if mn(1) == 1 then
                // min(A1, 'r') = A1(1,:) because A1 is a row vector
                if opts == "r" then
                    m = A1;
                    // min(A1, 'c') or min(A1, 'm') = min(A1)
                else
                    m = min(v', opts);
                    if m > 0 & length(v)<> mn(1)*mn(2) then
                        m = 0;
                    end
                end
            end

            // If mn(2) = 1, A1 is a column vector
            if mn(2) == 1 then
                if opts == "c" then
                    m = A1;
                else
                    m = min(v, opts);
                end
                if m > 0 & length(v)<> mn(1)*mn(2) then
                    m = 0;
                end
            end

            // Return a sparse vector containing the max in terms of 'c', 'r' or 'm'
            if mn(1) <> 1 & mn(2) <> 1 then
                m = spzeros(mn(1),1);
                // If opts = 'c', the result is returned in column vector
                if opts == "c" then
                    m = spzeros(mn(1),1);
                    for i = 1:mn(1)
                        pos = length(find(ij(:,1)==i));
                        if pos <> mn(2) then
                            m(i) = 0;
                        else
                            m(i) = min(A1(i,:));
                        end
                    end
                    // If opts = 'r' or 'm', the result is returned in row vector
                else
                    m = spzeros(1,mn(2));
                    for i = 1:mn(2)
                        pos = length(find(ij(:,2)==i));
                        if pos <> mn(1) then
                            m(i) = 0;
                        else
                            m(i) = min(A1(:,i));
                        end
                    end
                end
            end

        case 5
            // If the second argument is a sparse
            A2 = elements(2);

            // Check if A2 is a sparse
            if type(A2) <> 5 then
                error(msprintf(_("%s: Wrong type for input argument #%d: A sparse matrix expected.\n"), "%sp_min", 2));
            end

            [m1, n1] = size(A1);
            [m2, n2] = size(A2);

            // Check the size of A2
            if (m1 <> m2 | n1 <> n2) then
                error(msprintf(_("%s: Wrong size of input argument #%d: Same size as input argument #%d expected.\n"), "%sp_min", 2, 1));
            end

            // Retrieve the indices of non-zeros
            ij1 = spget(A1);
            ij2 = spget(A2);

            // A1 and A2 contain non zeros -> full
            if size(ij1,"r") == m1*n1 & size(ij2,"r") == m2*n2 then
                [m,k] = min(full(A1), full(A2));
                m = sparse(m);
                k = sparse(k);
            else
                m = A1;
                pos = find(m > A2);
                m(pos) = A2(pos);
            end

        else
            error(msprintf(_("%s: Wrong type for input argument #%d: A sparse matrix or a character expected.\n"), "%sp_min", 2));
        end

        // Case : max(A1,A2,A3,..,An) or max(list(A1,A2,A3,..,An))
    else
        if lhs == 2 then
            error(msprintf(_("%s: Wrong number of output argument: %d expected"), "%sp_min", 1));
        end
        // m is the first matrix
        m = elements(1);
        // Loop on the number of input arguments
        for i = 2:rhs
            An = elements(i);

            // Check if An is a sparse
            if type(An) <> 5 then
                if error_list then
                    error(msprintf(_("%s: Wrong type for input argument #%d (List element: %d): A sparse matrix expected.\n"), "%sp_min", 1, i))
                else
                    error(msprintf(_("%s: Wrong type for input argument #%d: A sparse matrix expected.\n"), "%sp_min", i))
                end
            end

            [m1, n1] = size(m);
            [m2, n2] = size(An);

            // Check size
            if (m1 <> m2 | n1 <> n2) then
                if error_list then
                    error(msprintf(_("%s: Wrong size of input argument #%d (List element: %d): Same size as input argument #%d expected.\n"), "%sp_min", 1, i, 1))
                else
                    error(msprintf(_("%s: Wrong size of input argument #%d: Same size as input argument #%d expected.\n"), "%sp_min", i, 1))
                end
            end

            ij1 = spget(m);
            ij2 = spget(An);

            if size(ij1,"r") == m1*n1 & size(ij2,"r") == m2*n2 then
                [m,k] = min(full(m), full(An));
                m = sparse(m);
                k = sparse(k);
            else
                pos = find(m > An);
                m(pos) = An(pos);
            end
        end
    end
endfunction
