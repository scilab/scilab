// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel GOUGEON
// Copyright (C) 2009 - Université du Maine - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [nb, loc] = members(A, S, varargin)
    //
    // Looks for how many times in S each element of A occurs
    // Looks for how many rows of S match each row of A
    // Can return the position in S of each respective first or last match.
    //
    // Syntax:
    // -------------------------
    // [nb [,loc]] = members(A, S)
    // [nb [,loc]] = members(A, S, "last")
    // [nb [,loc]] = members(A, S, "rows"|"cols")
    // [nb [,loc]] = members(A, S, "rows"|"cols", "last")
    // [nb [,loc]] = members(A, S, "rows"|"cols", "shuffle")
    // [nb [,loc]] = members(A, S, "rows"|"cols", "shuffle", "last")
    //
    // Input / Output arguments:
    // -------------------------
    // A   : Matrix or hypermatrix of booleans, integers, reals, complexes, polynomials or strings:
    //       entities (needles) to search in S (haystack).
    // S   : Matrix or hypermatrix of same datatype as A
    // "last": litteral keyword (optional): if it is provided, indices of last
    //       occurrences are returned instead of for the first ones.
    // "rows": litteral keyword (optional). if it is provided, each row of A
    //       is searched as a whole, among rows of S.
    // "cols": litteral keyword (optional). if it is provided, each column of A
    //       is searched as a whole, among columns of S.
    //       "rows" and "cols" options are exclusive. If both are specified,
    //       only the last specified one is considered.
    // "shuffle": litteral keyword (optional), modifying the "rows" or "cols"
    //       option. If it is provided, detection of -- say -- A rows in S is
    //       performed without respect of the order of elements in each row.
    //       This option is ignored when processing polynomials.
    //
    // NORMAL mode: neither "rows" nor "cols" is set. Then,
    // nb  : Matrix of reals: same sizes as A
    //       nb(i, j, ...): number of occurrences of A(i, j, ...) in S.
    // loc : Matrix of reals: same sizes as A
    //       loc(i, j, ...): linear index in S of the first occurrence of A(i, j, ...).
    //       If "last" is set, the index of the last occurrence is returned instead.
    //       loc(i, j, ...) returns zero if A(i, j, ...) is not found.
    // ROW-WISE mode:
    // nb  : Row of reals with size(A, 1) elements
    //       nb(i): number of occurrences of A(i, :) found as rows of S
    // loc : Row of reals with size(A, 1) elements
    //       loc(i): index of the first row in S which matches A(i, :).
    //       If "last" is set, the index of the last occurrence is returned instead.
    //       loc(i, j, ...) returns zero if A(i, :) is not found.
    // COLUMN-WISE mode:
    //  same as above. nb and loc are row vectors with size(A, 2) elements.
    //
    // REMARK: %inf, -%inf values are supported in A as well as in S.
    //
    // LIMITATION: in normal element-wise mode, %nan are supported only in A.
    //
    // Examples:
    // ---------
    // a) with reals:
    //   N = [ 7  3
    //       %inf 0
    //       %nan 1 ];
    //   H = [ 5   8    0   4
    //         3   4    7   7
    //         3 %inf %inf  2
    //         7   5    5   8 ];
    //   [nb, loc] = members(N, H)
    //   [nb, loc] = members(N, H, "last")
    //
    // b) with hypermatrices, from previous N and H:
    //   N = matrix(N, [3 1 2]);
    //   H = matrix(H, [4 2 2]);
    //   [nb, loc] = members(N, H, "last")
    //
    // c) with integers:
    //   N = int8(grand(3, 2, "uin", -5, 5));
    //   H = int8(grand(4, 4, "uin", -5, 5));
    //   [nb, loc] = members(N, H)
    //
    // d) with polynomials (complex coefficients are accepted):
    //   z = %z;
    //   N = [z (1-z)^2 ; -4 %i*z ];
    //   H = [2  %i*z -z  3-z  z  z^3 z];
    //   [nb, loc] = members(N, H)
    //
    // e) with text:
    //   N = [ "Hi" "Hu" "Allo"];
    //   H = [ "Hello" "Bonjour" "Allo"
    //         "Holà"  "Allo"  "Hallo"
    //         "Hi"    "Hé"    "Salud" ];
    //   [nb, loc] = members(N, H, "last")
    //
    // f) by rows:
    //   H = [
    //    3  3  0
    //    4  1  0
    //    2  0  3
    //    0  1  4
    //    3  4  3
    //    0  1  4
    //    3  1  0 ];
    //   N = [
    //    1 2 3
    //    0 1 4
    //    3 0 3
    //    4 1 0
    //    2 0 2 ];
    //   N, H
    //   [nb, loc] = members(N, H, "rows")
    //   [nb, loc] = members(N, H, "rows", "last")
    //   [nb, loc] = members(N, H, "rows", "shuffle")
    //
    // g) by columns, from previous N and H:
    //   N = N.', H = H.'
    //   [nb, loc] = members(N, H, "cols", "shuffle")

    [lhs, rhs] = argn();
    nb = [];
    if rhs == 0 then
        head_comments("members");
        return
    end
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): at least %d expected.\n"), "members", 2));
    end
    if A == [] then
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

    sA = size(A);
    type_A = type(A(:));
    type_S = type(S(:));
    if type_A ~= type_S then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: expected same type as first argument.\n"), "members", 2));
    end
    if and(type_A ~= [1 2 4 8 10]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of integers, reals, complexes, booleans, polynomials or strings expected.\n"), "members", 1));
    end
    if and(type_S ~= [1 2 4 8 10]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of integers, reals, complexes, booleans, polynomials or strings expected.\n"), "members", 2));
    end
    if or(isnan(S)) then
        error(msprintf(gettext("%s: Wrong value for argument #%d: Must not contain NaN.\n"), "members", 2));
    end

    // Checking optional flags (if any):
    last = %f;
    direction = "";
    shuffle = %f;
    shuffle_temp = %f;
    if  rhs > 2 then
        i = 2;
        for option=varargin
            i = i + 1;
            if typeof(option) ~= "string" | ~isscalar(option) then
                msg = _("%s: Wrong type for argument #%d: Scalar string expected => option ignored.\n");
                warning(msprintf(msg, "members", i))
                continue
            end
            o = convstr(option, "l");
            if o == "last" then
                last = %t;
            elseif o == "rows"
                direction = "rows";
            elseif o == "cols"
                direction = "cols";
            elseif o == "shuffle"
                shuffle_temp = %t;
            else
                msg = _("%s: Unknown option ""%s"" => ignored.\n");
                warning(msprintf(msg, "members", o))
            end
        end
        if shuffle_temp == %t then
            if direction == "" then
                msg = _("%s: Option ""%s"" only relevant for ""%s"" and ""%s"" modes => ignored.\n");
                warning(msprintf(msg, "members", "shuffle", "rows", "cols"))
            else
                shuffle = %t;
            end
        end
    end

    // ------------------------------------------------------------------------

    // Usual processing: searching for matching individual components
    // --------------------------------------------------------------
    if direction == "" then
        // Reals: special faster processing with dsearch().
        //  %inf, -%inf are supported in A and S. %nan are only supported in A.
        //  Processing integers and strings requires bugs 6305 & 12778 to be fixed.

        if type_A == 8 then   // Convert integers into reals in order to use dsearch
            A = double(A);
            S = double(S);
        elseif type_A == 4 then   // Convert booleans into reals
            A = bool2s(A);
            S = bool2s(S);
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
                    if ~isempty(k)
                        loc(k) = length(S)-loc(k)+1;
                    end
                end
                loc = matrix(loc, size(A));
            end
            // ------------------------------------------------------------------------
        else
            // Other cases : polynomials, text, complexes
            // ==========================================
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
            //   nb  : j doubles (occupied at the end)
            //   loc : j doubles (occupied at the end)
            //   tmp : LS * (j-i+1) doubles needed
            //   A2  : LS * mem(A(i:j)) needed
            //   S2  : mem(S)*(j-i+1) needed

            // Function setting the thickness of the slice, according to the
            //    available and needed memory
            function j = set_j(i, LS, memS, rA)
                // rA : remaining unprocessed part of A
                e = size(rA, "*");
                Vtest = rA(1:e);
                memrA = varspace("Vtest");   // [bytes]
                j = i+e-1;
            endfunction

            memS = varspace("S");  // [bytes]

            // Starting and ending indices in A for the current slice
            i = 1;   // Starting index: initialization
            j = 0;   // Ending index: initialization
            while j<LA      // Slicing loop
                // Setting next j (=> thickness of the slice = j-i+1)
                j = set_j(i, LS, memS, A(i:$))

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
                    if ~isempty(loc(k))
                        loc(k) = LS - loc(k) + 1;
                    end
                end
                loc = matrix(loc, sA);
            end
        end
        // ========================================================================

    else
        // Row-wise processing: searching for matching rows
        // ------------------------------------------------

        // Additional input checking:
        A = squeeze(A);
        if ~ismatrix(A) then
            msg = _("%s: Wrong type for argument #%d: Matrix expected.\n"); // error #209
            error(msprintf(msg, "members", 1))
        end

        S = squeeze(S);
        if ~ismatrix(S) then
            msg = _("%s: Wrong type for argument #%d: Matrix expected.\n"); // error #209
            error(msprintf(msg, "members", 2))
        end

        if direction == "rows" & size(A, 2) ~= size(S, 2) then
            msg = _("%s: Incompatible input arguments #%d and #%d: Same number of columns expected.\n");
            error(msprintf(msg, "members", 1, 2))
        elseif direction == "cols" & size(A, 1) ~= size(S, 1) then
            msg = _("%s: Incompatible input arguments #%d and #%d: Same number of rows expected.\n");
            error(msprintf(msg, "members", 1, 2))
        end

        // Column-wise = Row-wise after transposition
        if direction == "cols" then
            A = A.';
            S = S.';
        end

        // If "shuffle" is set, we first sort elements along each row of A and S
        if  type_A ~= 2 & shuffle then    // Polynomials are not sortable
            A = gsort(A, "c", "i");
            S = gsort(S, "c", "i");
        end

        // Pre-Processing: Preparing initial chart of double indices
        nLA = size(A, 1);  // Number of rows of A
        nLS = size(S, 1);  // Number of rows of S
        I1 = (1:nLA)' .*. ones(nLS, 1); // indices of A rows, grouped by S rows
        I2 = ones(nLA, 1) .*. (1:nLS)';  // set of S rows indices, replicated by the number of A rows
        IND = [I1 I2];   // Column #1 gives index in A. column #2 gives index in S

        // Processing: loop over columns
        for k = 1:size(S, 2)
            cA = A(IND(:, 1), k);
            cS = S(IND(:, 2), k);
            IND = IND(find(cA==cS), :);
            if IND == [] then
                break
            end
        end

        // Post-processing
        if last then
            [nb, loc] = members((1:nLA)', IND(:, 1), "last");
        else
            [nb, loc] = members((1:nLA)', IND(:, 1));
        end
        k = find(loc~=0);
        loc(k) = IND(loc(k), 2);
        nb = nb';
        loc = loc';
    end

endfunction
