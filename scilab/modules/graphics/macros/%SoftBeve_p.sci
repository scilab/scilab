// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2104 - Scilab Enterprises - Antoine ELIAS
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function %SoftBeve_p(t)
    count = size(t(1), "*");

    printf("SoftBevel Border\n");
    printf("  Type            : %s\n", sci2exp(t.type));

    if count == 2 then
        return
    end

    printf("  Highlight outer : %s\n", sci2exp(t.hlouter));

    if count == 4 then
        printf("  Shadow outer    : %s\n", sci2exp(t.shadowouter));
        return;
    end

    printf("  Highlight inner : %s\n", sci2exp(t.hlinner));
    printf("  Shadow outer    : %s\n", sci2exp(t.shadowouter));
    printf("  Shadow inner    : %s\n", sci2exp(t.shadowinner));
endfunction
