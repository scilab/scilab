// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge STEER <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [y, z] = %_filter(b, a, x, z)
    fname = "filter"
    [lhs, rhs] = argn(0)

    if rhs < 3 | rhs > 4
        error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"), fname, 3, 4));
    end

    if rhs == 3
        z = 0;
    end

    type_b = typeof(b);
    type_a = typeof(a);
    type_x = typeof(x);
    type_z = typeof(z);

    if type_b <> "constant" & type_b <> "polynomial"
        error(msprintf(_("%s: Wrong type for input argument #%d: Real matrix or polynomial expected.\n"), fname, 1));
    end

    if type_a <> "constant" & type_a <> "polynomial"
        error(msprintf(_("%s: Wrong type for input argument #%d: Real matrix or polynomial expected.\n"), fname, 2));
    end

    if type_x <> "constant"
        error(msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 3));
    end

    if type_z <> "constant"
        error(msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 4));
    end

    if ~isreal(b)
        error(msprintf(_("%s: Wrong type for input argument #%d: Real matrix or polynomial expected.\n"), fname, 1));
    end

    if ~isreal(a)
        error(msprintf(_("%s: Wrong type for input argument #%d: Real matrix or polynomial expected.\n"), fname, 2));
    end

    if ~isreal(x)
        error(msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 3));
    end

    if ~isreal(z)
        error(msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 4));
    end

    if (size(b, "c") <> 1) & (size(b, "r") <> 1)
        error(msprintf(_("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 1));
    end

    if (size(a, "c") <> 1) & (size(a, "r") <> 1)
        error(msprintf(_("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 2));
    end

    if (size(x, "c") <> 1) & (size(x, "r") <> 1)
        error(msprintf(_("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 3));
    end

    if (size(z, "c") <> 1) & (size(z, "r") <> 1)
        error(msprintf(_("%s: Wrong size for input argument #%d: Vector expected.\n"), fname, 4));
    end

    // User mixes polynomial and vector notation
    if type_b == "polynomial" & size(a, "*") <> 1
        error(msprintf(_("%s: Incompatible input arguments #%d and #%d: a polynomial and 1-by-1 matrix or two polynomials expected.\n"), fname, 1, 2));
    end

    // User mixes polynomial and vector notation
    if type_a == "polynomial" & size(b, "*") <> 1
        error(msprintf(_("%s: Incompatible input arguments #%d and #%d: a polynomial and 1-by-1 matrix or two polynomials expected.\n"), fname, 1, 2));
    end

    if type_b == "polynomial" | type_a == "polynomial"
        c = b/a;
        if type_a=="constant"
            b = b/a
            a = ones(a)
        else
            b = c.num;
            a = c.den;
        end
        deg_b = degree(b);
        deg_a = degree(a);
        deg = max(0,max(deg_b, deg_a));
        b = coeff(b, deg:-1:0);
        a = coeff(a, deg:-1:0);
    end

    if rhs == 4 then
        [y, z] = filter(b, a, x, z);
    else
        [y, z] = filter(b, a, x);
    end

endfunction
