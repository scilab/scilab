// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2012 - Cedric Delamarre
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
