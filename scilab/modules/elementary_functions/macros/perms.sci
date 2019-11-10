// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = perms(x, unic, self, sortable)

    // self, sortable: internal arguments for recursivity

    // PERMS function
    // Given an array x of size("*") = n, perms(x) returns the n! possible
    //  permutations of x components.
    // Inputs :
    //  x : scalars/vectors
    // output :
    // y : matrix

    // Top level instructions
    // ----------------------
    if ~isdef("self","l") | type(self)<>4 then
        rhs = argn(2);
        if rhs < 1 | rhs > 2 then
            error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"),"perms", 1, 2));
        end
        xsize = size(x,"*")
        if xsize < 2 then
            y = x
            return
        end
        x = x(:).'
        unic = isdef("unic","l")
        if unic
            tx = type(x)
            sortable = or(tx==[4 8 10]) | (or(tx==[1 5]) && isreal(x))
            if sortable
                if tx==4 // gsort() does not yet support booleans
                    x = gsort(iconvert(x,1),"g","i")==int8(1)
                else
                    x = gsort(x,"g","i")
                end
                // Let's compute the expected number of permutations
                    // tags for bounds after sorting
                if or(tx==[1 5 8])
                    joker = [x(1)-1, x($)+1]
                elseif tx==4
                    joker = ~[x(1) x($)];
                else // text
                    joker = [x(1) x($)] + "a";
                end
                    // groups of duplicates
                gl = find([x joker(2)]<>[joker(1) x]);
                gl = gl(2:$) - gl(1:$-1); // lengths of groups of duplicates
                nperms = factorial(xsize);
                if and(gl==1)       // There are only singletons
                    unic = %f       // => the classical algo is faster
                    x = x($:-1:1)   // But we match the order when "unique"
                else
                    nperms = nperms / prod(factorial(gl));
                end
                perms_checkMemory(x, xsize, nperms)
            else
                msg = _("perms: The ""unique"" option can be used only for sortable real numbers, booleans or texts.\n")
                error(msprintf(msg))
            end
        elseif xsize > 8  // 8! = 40320  9! = 362880
            perms_checkMemory(x, xsize, factorial(xsize))
        end

        if (recursionlimit() - recursionlimit("current")) < xsize
            oldRecursionlimit = recursionlimit()
            recursionlimit(recursionlimit("current") + xsize + 1)
        end
    else
        xsize = size(x,"*")
    end


    // PROCESSING
    // ==========
    if unic & sortable
        // All DISTINCT permutations
        // -------------------------
        if xsize==3
            y = [x([1 2 3]);
                 x([1 3 2]);
                 x([2 1 3]);
                 x([2 3 1]);
                 x([3 1 2]);
                 x([3 2 1])];
            // y = matrix(x([1 2 3 ; 1 3 2; 2 1 3 ; 2 3 1 ; 3 1 2; 3 2 1]), 6, -1)
            b = (1:6)<0
            b([3 4 6]) = x(1)==x(2)
            b([4 5 6]) = b([4 5 6]) | x(1)==x(3)
            b([2 5 6]) = b([2 5 6]) | x(2)==x(3)
            y(b,:) = []
        elseif xsize <> 2
            k = [0 find(x(2:$)<>x(1:$-1)) xsize] + 1
            y = []
            for i = 1:length(k)-1
                s = x; s(k(i)) = [];
                np = 1
                if xsize==2
                    p = s
                elseif xsize==3
                    if s(1)==s(2)
                        p = s
                    else
                        p = [s ; s([2 1])]
                        np = 2
                    end
                else
                    p = perms(s, unic, %t, %t)
                    np = size(p,1)
                end
                if type(x)==10
                    h = x(k(i)) + emptystr(np, 1)
                else
                    h = x(k(i)) * ones(np,1)
                end
                y = [y ; [h p]]
            end
        else // xsize==2 // we have necessarily x(1)==x(2) (otherwise => unic=%f)
            y = x
            //else
            //    y = [x([2 1]) ; x]
            //end
        end
    else
        // All permutations
        // ----------------
        if xsize==3
            y = [x([3 2 1])
                 x([3 1 2]);
                 x([2 3 1]);
                 x([2 1 3]);
                 x([1 3 2]);
                 x([1 2 3])];
            // y = matrix(x([1 2 3 ; 1 3 2; 2 1 3 ; 2 3 1 ; 3 1 2; 3 2 1]), 6, -1)
        elseif xsize <> 2
            indrec = perms(1:xsize-1);
            ind = [ones(prod(1:xsize-1),1)*xsize, indrec];
            for j = xsize-1:-1:1
                indrec(indrec==j) = j + 1;
                ind = [ind; [ones(prod(1:xsize-1),1)*j,indrec]];
            end
            y = matrix(x(ind), size(ind));
        else   // xsize == 2
            y = [x([2 1]) ; x]
        end
    end

    if isdef("oldRecursionlimit","l") then
        recursionlimit(oldRecursionlimit)
    end
endfunction
// ----------------------------------------------------------------------------

function perms_checkMemory(x, xsize, nperms)
    // Checks available RAM. Yields an error if not enough memory
    // nperms: expected number of permutations
    // xsize = size(x,"*")

    m = getmemory()*1024 / 3  // free RAM / 3 (for intermediate memory)

    // Required memory for each row = for each permutation
    select type(x)
    case 1
        wr = 8*xsize

    case 4
        wr = 4*xsize

    case 5  // sparse
        wr = nnz(x)*12   // 8 bytes for each value, 4 for each address

    case 8  // integers
        b = inttype(x)
        if b>10, b = b-10, end
        wr = b*xsize

    case 10 // text => assessment:
        wr = sum(length(x)) + xsize // lower bound, for only ascii chars

    else    // polynomials, cells ...
        [names, wr] = who('local');
        wr = wr(find(names=="x"))
    end

    nsr = m / wr     // number of storable rows
    if nsr < nperms
        msg = _("perms: Too many permutations (%d). Would run out of memory.\n")
        error(msprintf(msg, nperms))
    end
endfunction
