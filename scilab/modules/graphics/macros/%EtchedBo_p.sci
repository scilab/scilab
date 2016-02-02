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
function %EtchedBo_p(t)
    count = size(t(1), "*");

    printf("Etched Border\n");

    if count == 1 then
        printf("  No option\n");
        return;
    end

    if count == 2 then
        printf("  Type      : %s\n", sci2exp(t.type));
        return;
    end

    if count == 3 then
        printf("  Highlight : %s\n", sci2exp(t.hl));
        printf("  Shadow    : %s\n", sci2exp(t.shadow));
        return;
    end

    printf("  Type      : %s\n", sci2exp(t.type));
    printf("  Highlight : %s\n", sci2exp(t.hl));
    printf("  Shadow    : %s\n", sci2exp(t.shadow));
endfunction
