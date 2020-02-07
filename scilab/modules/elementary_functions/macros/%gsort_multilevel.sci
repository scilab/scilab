// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have
// received along with this program.

function [sorted, K] = %gsort_multilevel(array, sortype, sortdir, criteria)
    // This internal overload sorts only dense matrices.
    // For hypermatrices, %hm_gsort() is called upstream
    //   - to reformat the input as a matrix
    //   - to call %_gsort() with the equivalent input matrix
    //   - to reformat the output as expected
    //   There is nothing specific to complex numbers with hypermatrices
    // For sparse matrices, %sp_gsort() is called upstream.
    //
    // array   : vector or matrix to be sorted.
    //           Hypermatrices are pre- and post-processed by %hm_gsort()
    // sortype : "g" "r" "c" "lr" "lc". Default "g"
    // sortdir: [], or vector of "i" or "d". Default = "d"
    // criteria: list of Scilab functions or primitives handles, or :.
    //           When a function fun requires some additional parameters
    //           a, b, c, ... list(fun, a, b, c,..) must be provided
    //           instead of only fun.

    sa = size(array)

    // CHECKING INPUT PARAMETERS
    // -------------------------
    // array:
    // This overload is called only when array is defined and are complex numbers

    // sortype:
    if ~isdef("sortype", "l") || sortype==[] || (type(sortype)==10 && sortype(1)=="")
        sortype = "g"
    elseif type(sortype)~=10
        msg = _("%s: Argument #%d: Text(s) expected.\n")
        error(msprintf(msg, "gsort", 2))
    else
        sortype = convstr(sortype(1))
        if ~or(sortype==["g" "r" "c" "lr" "lc"])
            msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
            error(msprintf(msg, "gsort", 2, "''g'',''r'',''c'',''lc'',''lr''"))
        end
    end

    // sortdir:
    if ~isdef("sortdir", "l") || sortdir==[]
        sortdir = "d"           // for back-compatibility
    elseif type(sortdir)~=10
        msg = _("%s: Argument #%d: Text(s) expected.\n")
        error(msprintf(msg, "gsort", 3))
    else
        sortdir = convstr(sortdir)
        k = find(sortdir <> "i" & sortdir <> "d")
        if k <> []
            msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
            error(msprintf(msg, "gsort", 3, "''i'',''d''"))
        end
    end

    // criteria:
    if type(criteria) <> 15 then
        msg = _("%s: Argument #%d: List expected.\n")
        error(msprintf(msg, "gsort", 4))
    end
    if size(criteria) <> size(sortdir,"*") then
        msg = _("%s: Arguments #%d and #%d: Same numbers of elements expected.\n")
        error(msprintf(msg, "gsort", 3, 4))
    end
    for c = criteria
        t = type(c)==13 || typeof(c)=="fptr" || ..
           (typeof(c)=="implicitlist" && (1:1:$)==c)
        if ~t & type(c)==15
            if length(c) > 0
                t = type(c(1))==13 || typeof(c(1))=="fptr" || ..
                   (typeof(c(1))=="implicitlist" && (1:1:$)==c(1))
            end
        end
        if ~t
            msg = _("%s: Argument #%d: List of functions identifiers expected.\n")
            error(msprintf(msg, "gsort", 4))
        end
    end

    // ONLY ONE LEVEL => SIMPLE DIRECT PROCESSING
    // ------------------------------------------
    if size(criteria)==1 then
        fun = criteria(1)
        if typeof(fun)=="implicitlist" & fun==(1:1:$)
            [sorted, K] = gsort(array, sortype, sortdir)
        else
            v = %gsort_eval(array, fun)
            [sorted, K] = gsort(v, sortype, sortdir)
            select sortype
            case "g"
                sorted = matrix(array(K), size(array))
            case "r"
                C = ones(sa(1),1) * (1:sa(2))
                sorted = matrix(array(K+(C-1)*sa(1)), sa)
            case "c"
                R = (1:sa(1))' * ones(1,sa(2))
                sorted = matrix(array(R+(K-1)*sa(1)), sa)
            case "lr"
                sorted = array(K, :)
            case "lc"
                sorted = array(:, K)
            end
        end
        if type(array)==1 & ~isreal(array) & isreal(sorted)
            sorted = complex(sorted, 0)
        end
        return
    end

    // OTHERWISE:
    // BUILDING THE MATRIX OF SEPARATE RANKS
    // -------------------------------------
    nbcrit = length(criteria)
    a = array(:)
    kk = []
    for i = 1:nbcrit
        fun = criteria(i)
        v = %gsort_eval(a, fun)
        [vs, k0] = gsort(v(:), "g", sortdir(i))
        [?, k2] = gsort(k0,"g","i")
        // The k(i) of equal elements following the first one must be
        //  set to the heading index:
        b = [%T ; vs(2:$) <> vs(1:$-1)];
        k = cumsum(b);  // Done!
        kk = [kk k(k2)];
    end

    // SORTING
    // -------
    if sortype=="g" then
        [?, K] = gsort(kk, "lr", "i");
        K = matrix(K, sa)
        sorted = matrix(array(K), sa)

    elseif or(sortype==["c" "r"]) then
        [?, K] = gsort(kk, "lr", "i");
        K = matrix(K, sa)
        z = zeros(sa(1),sa(2))
        z(K) = 1:prod(sa)
        [?, K] = gsort(z, sortype, "i")
        if sortype=="c" then
            R = (1:sa(1))' * ones(1,sa(2))
            sorted = matrix(array(R+(K-1)*sa(1)), sa)
        else // "r"
            C = ones(sa(1),1) * (1:sa(2))
            sorted = matrix(array(K+(C-1)*sa(1)), sa)
        end

    elseif sortype=="lr" then
        tmp = ones(nbcrit,1)*(1:sa(2))+(0:nbcrit-1)'*ones(1,sa(2))*sa(2)
        tmp = matrix(kk, sa(1), -1)(:,tmp)
        [?, K] = gsort(tmp, "lr", "i");
        sorted = array(K, :)

    else // sortype=="lc"
        tmp = matrix(kk',nbcrit*sa(1), -1)'
        [?, K] = gsort(tmp, "lr", "i");
        K = K'
        sorted = array(:,K)
    end

    if type(array)==1 & ~isreal(array) & isreal(sorted)
        sorted = complex(sorted, 0)
    end
endfunction
// ------------------------------------------------------
function v = %gsort_eval(a, fun)
    if type(fun)==15
        params = fun
        params(1) = null()
        fun = fun(1)
        if typeof(fun)=="fptr" & fun==atan & type(a)==1 & ~isreal(a)
            v = fun(imag(a), real(a)) + params(1)
        else
            v = fun(a, params(:))
        end
    elseif typeof(fun)=="fptr" & fun==atan & type(a)==1 & ~isreal(a)
        v = atan(imag(a), real(a))
    elseif typeof(fun)=="implicitlist" & (1:1:$)==fun
        v = a
    else
        v = fun(a)
    end
endfunction
