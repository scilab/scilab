// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2104 - Scilab Enterprises - Antoine ELIAS
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
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
