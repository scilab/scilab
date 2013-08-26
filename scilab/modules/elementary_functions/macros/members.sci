// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009,2013 - Université du Maine - Samuel Gougeon
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [nb, loc] = members(A, S, last)
    //
    // Input / Output arguments:
    // -------------------------
    // A   : Matrix or hypermatrix of booleans, integers, reals, complexes, polynomials or strings:
    //       entities (needles) to search in S (haystack).
    // S   : Matrix or hypermatrix of same datatype as A
    // last: Scalar boolean
    // nb  : Matrix of reals: same sizes as A
    //       nb(i, j, ...): number of occurrences of A(i, j, ...) in S.
    // loc : Matrix of reals: same sizes as A
    //       loc(i, j, ...): linear index in S of the first occurrence of A(i, j, ...).
    //       If last==%t, the index of the last occurrence is returned instead.
    //       loc(i, j, ...) returns zero if A(i, j, ...) is not found.
    //
    // %inf, -%inf values are supported in A as well as in S.
    // %nan are supported only in A.
    //
    // Examples:
    // ---------
    // a) with reals:
    // N = [ 7  3
    //     %inf 0
    //     %nan 1 ];
    // H = [ 5   8    0   4
    //       3   4    7   7
    //       3 %inf %inf  2
    //       7   5    5   8 ];
    // [nb, loc] = members(N, H)
    // [nb, loc] = members(N, H, %T)
    //
    // b) with hypermatrices, from previous N and H:
    // N = matrix(N, [3 1 2]);
    // H = matrix(H, [4 2 2]);
    // [nb, loc] = members(N, H, %T)
    //
    // c) with integers:
    // N = int8(grand(3, 2, "uin", -5, 5));
    // H = int8(grand(4, 4, "uin", -5, 5));
    // [nb, loc] = members(N, H)
    //
    // d) with polynomials (complex coefficients are accepted):
    // z = %z;
    // N = [z (1-z)^2 ; -4 %i*z ];
    // H = [2  %i*z -z  3-z  z  z^3 z];
    // [nb, loc] = members(N, H)
    //
    // e) with text:
    // N = [ "Hi" "Hu" "Allo"];
    // H = [ "Hello" "Bonjour" "Allo"
    //       "Holà"  "Allo"  "Hallo"
    //       "Hi"    "Hé"    "Salud" ];
    // [nb, loc] = members(N, H, %t)

    [lhs, rhs] = argn();
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): at least %d expected.\n"), "members", 2));
    end
    if rhs == 2 then
        last = %f;
    end
    if A == [] then
        nb = [];
        if lhs > 1 then
            loc = [];
        end
        return
    end
    if  S == []  then
        nb = zeros(A);
        if lhs > 1 then
            loc = zeros(A);
        end
        return
    end

    type_A = type(A(:));
    type_S = type(S(:));
    if type_A <> type_S then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: expected same type as first argument.\n"), "members", 2));
    end
    if and(type_A <> [1 2 4 8 10]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of Integers, Reals, Complexes, Booleans, Polynomials or Strings expected.\n"), "members", 1));
    end
    if and(type_S <> [1 2 4 8 10]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of Integers, Reals, Complexes, Booleans, Polynomials or Strings expected.\n"), "members", 2));
    end
    if or(isnan(S)) then
        error(msprintf(gettext("%s: Wrong value for argument #%d: Must not contain NaN.\n"), "members", 2));
    end
    if type(last) <> 4 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Boolean matrix expected.\n"), "members", 3));
    end
    if ~isscalar(last) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Scalar expected.\n"), "members", 3));
    end

    // ------------------------------------------------------------------------

    // Reals: special faster processing with dsearch().
    //  %inf, -%inf are supported in A and S. %nan are only supported in A.
    //  Processing integers and strings requires bugs 6305 & 12778 to be fixed.

    if type_A == 8 then   // Convert integers into reals in order to use dsearch
        A = double(A);
        S = double(S);
    end
    if type_A == 1 & isreal(A) & isreal(S) then
        S = S(:);
        if last then
            S = S($:-1:1);
        end
        [Su, kS] = unique(S);
        [i, nbS] = dsearch(S, Su, "d");

        I = dsearch(A(:), Su, "d");
        k = find(I~=0);
        nb = I;
        nb(k) = nbS(I(k));
        nb = matrix(nb, size(A));
        if lhs > 1 then
            loc = I;
            loc(k) = kS(I(k));
            if last then
                loc(k) = length(S)-loc(k)+1;
            end
            loc = matrix(loc, size(A));
        end
        // ------------------------------------------------------------------------
    else
        // Other cases : polynomials, text, complexes
        // ==========================================
        sA = size(A);
        LA = size(A, "*");
        LS = size(S, "*");
        A = A(:);
        if ~last then
            S = S($:-1:1);
        end

        // Slices may be needed in case of memory overflow
        //
        // Function returning the memory space in [bytes] occupied by each
        //  querried variable. If a variable exists in several contexts,
        //  only the last created one is considered
        function varargout = varspace(varargin)
            [Ln, Ls] = who("local");
            vs = list();
            for vn = varargin
                i = find(Ln==vn);
                if i ~= [] then
                    vs($+1) = Ls(i(1))*8;
                else
                    vs($+1) = 0;
                end
            end
            varargout = vs;
        endfunction
        // -----------------------------------
        // Memory space needed to process a i:j slice of A:
        // nb  : j doubles (occupied at the end)
        // loc : j doubles (occupied at the end)
        // tmp : LS * (j-i+1) doubles needed
        // A2  : LS * mem(A(i:j)) needed
        // S2  : mem(S)*(j-i+1) needed

        // Function setting the thickness of the slice, according to the
        //    available and needed memory
        function j = set_j(i, LS, memS, rA)
            // rA : remaining unprocessed part of A
            [memA2, memS2, memtmp] = varspace("A2", "S2", "tmp");
            // Available memory [bytes]:
            m = stacksize();
            avail = (m(1)-m(2))*8 + memA2 + memS2 + memtmp -16*(i-1); // [bytes]
            avail = 0.15*avail;  // A lot of memory is used as Intermediate memory
            // Diadic loop on e = j-i+1 (slice's thickness):
            // Init
            e = size(rA, "*");
            Vtest = rA(1:e);
            memrA = varspace("Vtest");   // [bytes]
            // Loop fitting the slice's thickness e for the available memory
            while (e*(memS+8*LS+16)+LS*memrA > avail & e > 1)
                e = ceil(e/2);   // Remaining thickness => divided by 2
                Vtest = rA(1:e);
                memrA = varspace("Vtest");
            end
            if (e*(memS+8*LS+16)+LS*memrA > avail) then
                msg = _("%s: not enough memory to proceed");
                error(sprintf(msg, "members"));
            end
            j = i+e-1;
        endfunction

        memS = varspace("S");  // [bytes]

        // Starting and ending indices in A for the current slice
        i = 1;   // Starting index: initialization
        j = 0;   // Ending index: initialization
        while j < LA      // Slicing loop
            // Setting next j (=> thickness of the slice = j-i+1)
            j = set_j(i, LS, memS, A(i:$));

            // Progression bar
            if j < LA then
                if isdef("waitH") then
                    waitbar(j/LA, waitH);
                elseif j/LA < 0.4   // Else: too few steps => not worthwhile
                    waitH = waitbar(j/LA);
                end
            end

            A2 = repmat(A(i:j), 1, LS);
            S2 = repmat(S(:).', j-i+1, 1);

            tmp = bool2s(A2==S2);
            nb(i:j) = sum(tmp, "c");
            if lhs > 1 then
                tmp = tmp.* ( (1:LS) .*. ones(j-i+1, 1) );
                loc(i:j) = max(tmp, "c");
            end
            i = j+1;
            // End of slice processing
        end
        // Deleting the waitbar (if any) and clearing transient variables
        clear A2 S2 tmp
        if isdef("waitH") then
            delete(waitH);
        end

        // Final operations on the overall result(s)
        nb = matrix(nb, sA);
        if lhs > 1
            if ~last
                k = loc~=0;
                loc(k) = LS - loc(k) + 1;
            end
            loc = matrix(loc, sA);
        end
    end

endfunction
