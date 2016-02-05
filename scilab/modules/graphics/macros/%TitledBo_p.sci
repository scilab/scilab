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
function %TitledBo_p(t)
    count = size(t(1), "*");

    printf("Titled Border\n");
    if count == 2 then
        if typeof(t(2)) == "string" then
            printf("  Title         : %s\n", sci2exp(t.title));
        else
            printf("  Border        : %s\n", typeof(t.border));
        end
        return;
    end

    printf("  Border        : %s\n", typeof(t.border));
    printf("  Title         : %s\n", sci2exp(t.title));

    if count < 5 then
        return;
    end

    printf("  Justification : %s\n", sci2exp(t.justification));
    printf("  Position      : %s\n", sci2exp(t.position));

    if count < 6 then
        return;
    end

    printf("  Font          : %s\n", typeof(t.font));

    if count < 7 then
        return;
    end

    printf("  Color         : %s\n", sci2exp(t.color));
endfunction
