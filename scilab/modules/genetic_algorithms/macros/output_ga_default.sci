// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Michael BAUDIN <michael.baudin@contrib.scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
// =============================================================================

function stop = output_ga_default(gen_index, nb_generation, Pop, FObj_Pop, param)
    printf(gettext("%s: iteration %d / %d \n"), "optim_ga", gen_index, nb_generation);
    printf(gettext("    min / max value found = %f / %f\n"), min(FObj_Pop), max(FObj_Pop));
    stop = %f
endfunction
