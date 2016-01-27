//   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
//   <bruno.pincon@iecn.u-nancy.fr>
//
// This set of scilab 's macros provide a few sparse utilities.
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

function [xn, rn] = rafiter(A, C, b, x0, nb_iter, verb)
    // raffinement iteratif
    //
    if ~exists("verb", "local") then , verb = %f , end
    if ~exists("nb_iter", "local") then , nb_iter = 2, end

    xn = x0
    for i=1:nb_iter
        rn = res_with_prec(A, xn, b)
        dx = taucs_chsolve(C, rn)
        if verb then
            crit1 = norm(rn) ; crit2 = norm(dx)
            mprintf(" it %2d : ||r|| = %e , ||dx|| = %e \n", i, crit1, crit2)
        end
        xn = xn - dx
    end
endfunction
