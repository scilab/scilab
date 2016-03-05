// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2014 - Pierre-Aime Agnel
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// =============================================================================

function stop = output_moga_default(gen_index, nb_generation, Pop, FObj_Pop, param)
    printf(gettext("%s: iteration %d / %d \n"), "optim_moga", gen_index, nb_generation);
    printf(gettext("    min / max value found = %f / %f\n"), min(FObj_Pop), max(FObj_Pop));
    stop = %f
endfunction
