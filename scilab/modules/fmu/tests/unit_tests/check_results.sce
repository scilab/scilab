//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2014 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
clear check_results;
function check_results(refs, outputs, outName, solver)
    // check the results with the reference results
    function ret = calc_err(i, refs, outputs)
        xp = refs.time;
        x = outputs.time;
        y = outputs.values(:,i);

        dk = splin(x, y);
        yp = interp(xp, x, y, dk);

        err = abs(refs.values - yp);
        m = mean(err, 'r');

        ret = [m, stdev(err, 'r', m)]
    endfunction

    outMin = min(outputs.values, 'r');
    outMax = max(outputs.values, 'r');

    mprintf("Type of solver: %s\n", solver);
    for i = 1:size(outName, "*")
        err = [calc_err(i, refs, outputs), outMin(i), outMax(i), outputs.values($, i)];
        mprintf("  %-20s mean %10.7f std %10.7f min %10.7f max %10.7f order %10.7f\n", ..
                outName(i), err(1), err(2), err(3), err(4), err(5));
    end
endfunction
