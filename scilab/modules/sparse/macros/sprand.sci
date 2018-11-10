// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function a = sprand(m, n, density, law)

    // CHECK ARGUMENTS
    // ---------------
    rhs = argn(2)
    if rhs < 3 | rhs > 4 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "sprand" , 3 , 4 ));
    end
    if rhs < 4 then
        law = "unf";
    end
    if m==0 | n==0 then
        a = sparse([], [], [0 0])
        return
    end
    // law
    if type(law)<>10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"sprand",4));
    end
    if and(law<>["u" "unf" "uniform" "def" "n" "nor" "normal"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"),"sprand",4,"uniform","normal"));
    end
    if or(law == ["u" "uniform" "def"]) then
        law = "unf";
    elseif or(law == ["n" "normal"]) then
        law = "nor";
    end

    // INITIALIZATION
    // --------------
    N = m*n
    density = max(min(density,1),0);
    nel = round(N*density); // the objective number of non zero elements
    if nel==0 then
        a = sparse([],[], [m,n])
        return
    end

    // GENERATION OF RANDOM UNIQUE INDICES OF NON-ZERO VALUES
    // ------------------------------------------------------
    // generate a sequence of increments greater than 1 ------------------------
    di = N / nel
    mdist = 1/density //the mean distance between two consecutive non-zero values
    ij    = grand(nel*1.01, 1, "exp", (mdist-1))

    s = (nel + sum(ij(1:nel)))/(N-di/2)  // rescaling factor
    ij = 1 + ij/s              // rescales intervals & guaranties that indices are unique
    ij = floor(cumsum(ij));    // Intervals between indices => indices
    ij(find(ij > N)) = [];     // Garanties that linear indices are <= m*n
    nel = length(ij)
    ij = ind2sub([m,n], ij);   // => [I J] conversion

    // RESULT
    // ------
    a = sparse(ij, grand(nel, 1, law, 0, 1),[m,n]);
endfunction

