// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2012 - Cedric Delamarre
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function x = %_kron(a, b)

    rhs = argn(2)
    lhs = argn(1)

    if rhs <> 2 then
        error(999,msprintf(_("%s: Wrong number of output argument(s): %d expected.\n"), "kron", 2));
    end

    if lhs <> 1 then
        error(999,msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "kron", 1));
    end

    x = a .*. b;

endfunction
