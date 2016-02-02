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
function %GridBagC_p(t)
    printf("GridBag Layout\n");
    printf("  Grid          : %s\n", sci2exp(t.grid));
    printf("  Weight        : %s\n", sci2exp(t.weight));
    printf("  Fill          : %s\n", sci2exp(t.fill));
    printf("  Anchor        : %s\n", sci2exp(t.anchor));
    printf("  Padding       : %s\n", sci2exp(t.padding));
    printf("  PreferredSize : %s\n", sci2exp(t.preferredsize));
endfunction
