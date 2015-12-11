// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2104 - Scilab Enterprises - Antoine ELIAS
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function %Compound_p(t)
    count = size(t(1), "*");

    printf("Compound Border\n");

    if count == 1 then
        printf("  No option\n");
        return;
    end

    printf("  Outer : %s\n", typeof(t.outer));
    printf("  Inner : %s\n", typeof(t.inner));
endfunction
