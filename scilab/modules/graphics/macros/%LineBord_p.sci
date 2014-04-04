// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2104 - Scilab Enterprises - Antoine ELIAS
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function %LineBord_p(t)
    count = size(t(1), "*");

    printf("Line Border\n");
    printf("  Color     : %s\n", sci2exp(t.color));

    if count < 3 then
        return;
    end

    printf("  Thickness : %s\n", sci2exp(t.thickness));

    if count < 4 then
        return;
    end

    printf("  Rounded   : %s\n", sci2exp(t.rounded));
endfunction
