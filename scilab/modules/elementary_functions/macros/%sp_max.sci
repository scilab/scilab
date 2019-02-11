// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [m, k] = %sp_max(varargin)
    [lhs, rhs] = argn(0);

    elements = varargin;
    error_list = "";

    // If the first argument is a list, it retrieves the number of sparse
    // matrices in list
    if type(varargin(1)) == 15 then
        if rhs <> 1 then
            msg = _("%s: Wrong size of input argument: %d expected.\n")
            error(msprintf(msg, "%sp_max", 1))
        end

        rhs = length(varargin(1));
        elements = varargin(1);
        error_list = _(" (in list)");

        // If the second argument of list is not a sparse -> ERROR
        if rhs == 2 & type(elements(2)) <> 5 then
            msg = _("%s: Wrong type for input argument #%d: A sparse matrix expected.\n")
            error(msprintf(msg, "%sp_max", 1))
        end
    end

    A1 = elements(1);         // Is necessarily a sparse, due to the %sp_ prefix
    // Check if A1 is real
    if ~isreal(A1) then
        msg = _("%s: Argument #%d: Complex numbers not supported.\n")
        error(msprintf(msg, "%sp_max", 1));
    end
    siz = size(A1);

    // max(A)
    // ------
    if rhs==1
        if isempty(A1) then
            m = []
            k = []
            return
        end
        // Retrieves entries of sparse matrix
        [ij, v, mn] = spget(A1)

        // Matrix of zeros:
        if v==[] then
            m = 0
            k = 1
        else
            [m, k] = max(v);
            k = ij(k,:)
            if m<0 then
                i = find(A1(:)==0,1)
                if i~=[]
                    m = 0
                    k = i
                end
            end
        end
        if length(m)<2
            m = full(m)
        end
        if mn(1)>1 & mn(2)>1 & length(k)==1
            k = ind2sub(siz, k)
        elseif (mn(1)<2 | mn(2)<2) & length(k)==2
            k = sub2ind(siz,k)
        end

    // max(A, "r"|"c"|"m")
    // -------------------
    elseif rhs==2 & type(elements(2))==10
        opts = elements(2);
        if ~or(opts==["c" "r" "m"]) then
            msg = _("%s: Wrong value for input argument #%d: [''r'' ''c'' ''m''] expected.\n")
            error(msprintf(msg, "%sp_max", 2))
        end
        if isempty(A1) then
            m = A1
            k = []
            return
        end
        if siz(1)==1 & opts=="r"
            m = A1
            k = ones(1, siz(2))
        elseif siz(2)==1 & opts=="c"
            m = A1
            k = ones(siz(1), 1)
        else
            if or(opts==["r" "m"])
                A1 = A1'
            end
            [ij, v, mn] = spget(A1)
            // mprintf("""%s"" [%d, %d]\n", opts, siz(1), siz(2))
            m = spzeros(mn(1), 1)
            k = ones(mn(1), 1)
            kk = unique(ij(:,1))
            for j = kk'
                [V,K] = max(A1(j,:))    // handles Nan correctly
                m(j) = V
                k(j) = K
            end
            // If opts = 'r' or 'm', the result is returned in row vector
            if or(opts==["r" "m"]) then
                m = m'
                k = k'
            end
        end

    // max(A1,A2,...) or equivalently max(list(A1,A2,..))
    // --------------------------------------------------
    else
        // m is the first matrix
        m = elements(1);
        if lhs>1
            k = ones(m)
        end
        [m1, n1] = size(m)

        // Loop on the number of input arguments
        for i = 2:rhs
            An = elements(i)
            // Check if An is a sparse
            if and(type(An) <> [1 5])  | (type(An)==1 & or(size(An)~=[1 1])) then
                msg = _("%s: Wrong type for input argument #%d%s: A sparse matrix or a scalar expected.\n")
                error(msprintf(msg, "%sp_max", i, error_list))
            end
            // Check if An is real
            if ~isreal(An) then
                msg = _("%s: Argument #%d%s: Complex numbers not supported.\n")
                error(msprintf(msg, "%sp_max", i, error_list))
            end
            [m2, n2] = size(An)
            // Check size
            if (m1 <> m2 | n1 <> n2) & or([m2 n2]~=[1 1]) then
                msg = _("%s: Wrong size of input argument #%d%s: Same size as input argument #%d expected.\n")
                error(msprintf(msg, "%sp_max", i, error_list, 1))
            end

            // Processing:
            mNan = isnan(m);
            newNan = isnan(An);
            pos = (m < An) | (mNan & ~newNan) | ..
                  (mNan & newNan) // position of the last %nan, as with dense processing
            if or([m2 n2]~=[1 1])
                m(pos) = An(pos)
            else
                m(pos) = An
            end
            if lhs > 1
                k(pos) = i
            end
        end  // for
        if length(m)<2
            m = full(m)
        end
    end
endfunction
