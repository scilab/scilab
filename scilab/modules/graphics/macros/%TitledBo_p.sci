// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2104 - Scilab Enterprises - Antoine ELIAS
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
