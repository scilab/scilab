// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
// Copyrifht (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - 2020 - Samuel GOUGEON : complete rewritting
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [A, k] = %sp_gsort(A, sortype, sortdir, criteria)
    lhs = argn(1)
    k = 0

    // ===================
    // CHECKING PARAMETERS
    // ===================
    if ~isdef("sortype", "l") then
        sortype = "g"
    else
        sortype = convstr(sortype(1))
        if ~or(sortype == ["g", "r", "c", "lr", "lc"]) then
            msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
            error(msprintf(msg, "gsort", 2, """g"",""r"",""c"",""lr"",""lc"""))
        end
    end
    if ~isdef("sortdir", "l")
        sortdir = "d"
    else
        sortdir = convstr(sortdir)
        if and(sortdir <> "d" & sortdir <> "i") then
            msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
            error(msprintf(msg, "gsort", 3, """i"",""d"""))
        end
        if ~isdef("criteria","l")
            sortdir = sortdir(1)
        end
    end

    // ==========
    // PROCESSING
    // ==========
    if sortype=="c"
        A = A.'
    end
    // Gets non zero values by increasing linearized indices:
    [ij, v, mn] = spget(A.');
    ij = ij(:,[2 1]);
    mn = mn([2 1]);

    s = prod(mn)

    // ------------------------
    // "g" general sorting mode
    // ------------------------
    if sortype=="g" then
        v($+1) = 0        // To get the position of all sorted implicit zeros
        ij($+1,:) = [mn(1)+1 1]  // (the value does not matter)

        // Sorting non zero values:
        if lhs == 1 then
            if ~isdef("criteria", "l")
                v = gsort(v, "g", sortdir)
            else
                v = gsort(v, "g", sortdir, criteria)
            end
        else
            if ~isdef("criteria", "l")
                [v, ks] = gsort(v, "g", sortdir)
            else
                [v, ks] = gsort(v, "g", sortdir, criteria)
            end
        end

        kz = find(v==0)  // Here is the position of zeros
        v(kz) = []       // Cleaning
        K = [ 1:kz-1  s-length(v)+kz:s]  // We build K
        Ain = A
        A = sparse(ind2sub(mn,K), v, mn) // We build the sorted sparse
        // Building the dense matrix of initial indices of sorted elements
        // A new "sparse_k" option could be implemented later to return a sparse k
        if lhs==2
            ks(kz) = []
            k = zeros(A);
            k(K) = sub2ind(mn, ij(ks,:));
            k(k==0) = find(Ain(:)==0)
            k = matrix(k, size(A))
        end
        return
    end

    // -------------------------------------
    // Sorting inside rows or inside columns
    // -------------------------------------
    if or(sortype==["r" "c"]) then   // "r" sorts rows of each column
        a = 2;                       // "c" sorts columns of each row
        uc = unique(ij(:,a))
        V = []
        K = []
        Kin = (1:mn(1))'*ones(1,mn(2))
        for n = uc'
            vec = A(:, n)
            if lhs==1
                if ~isdef("criteria", "l")
                    v = gsort(vec, "g", sortdir)
                else
                    v = gsort(vec, "g", sortdir, criteria)
                end
            else
                if ~isdef("criteria", "l")
                    [v, k] = gsort(vec, "g", sortdir)
                else
                    [v, k] = gsort(vec, "g", sortdir, criteria)
                end
            end
            [tmp, v] = spget(v);
            tmp(:, a) = n
            K = [K ; tmp]
            V = [V ; v]
            if lhs>1
                Kin(:,n) = k(:)
            end
        end
        A = sparse(K, V, mn);
        if lhs>1
            k = matrix(Kin, mn);
        end
        if sortype=="c"
            A = A.'
            k = k.'
        end
        return
    end

    // ---------------------
    // Lexicographic sorting
    // ---------------------
    msg = _("%s: Argument #%d: Complex sparse not yet supported in ""%s"" mode.\n")

    // Vector = special simple case
    // ----------------------------
    if isvector(A) then
        isRow = isrow(A)
        if (isRow & sortype=="lr") | (iscolumn(A) & sortype=="lc")
            k = 1
        else
            if lhs==1
                if ~isdef("criteria", "l")
                    A = gsort(A(:), "g", sortdir)
                else
                    A = gsort(A(:), "g", sortdir, criteria)
                end
            else
                if ~isdef("criteria", "l")
                    [A, k] = gsort(A(:), "g", sortdir)
                else
                    [A, k] = gsort(A(:), "g", sortdir, criteria)
                end
            end
            if isRow
                A = matrix(A, 1, -1)
                k = matrix(k, 1, -1)
            end
        end
        return
    end

    // "lr" case
    // ---------
    if sortype=="lc" then
        A = A.'
    end
    if ~isdef("criteria", "l")
        [A, k] = %sp_gsort_lr(A, sortdir);
    else
        [A, k] = %sp_gsort_lr(A, sortdir, criteria);
    end
    if sortype == "lc" then
        A = A.'
        k = matrix(k, 1, -1)
    end
endfunction

// ===================================================================

function [S, K] = %sp_gsort_lr(S, order, criteria)
    [nr,nc] = size(S)
    K = (1:nr)'
    crit = isdef("criteria","l")

    // List of column according to which sorting must be done
    J = 1:nc
        // We ignore columns that are uniform. Sorting them is useless
    Std = sum(S.^2,"r")/nr - (sum(S,"r")/nr).^2
    J(Std==0) = []

    // Processing (bulky. A more clever algo required (but hard))
    for j = J($:-1:1)
        if crit
            [?, k] = gsort(S(K, j), "g", order, criteria)
        else
            [?, k] = gsort(S(K, j), "g", order)
        end
        K = K(k,1)
    end
    S = S(K,:)
endfunction
