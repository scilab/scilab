// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Internal function

function matrix_out = atomsLoadLoad()

    // Init the output argument
    // =========================================================================
    matrix_out = [];

    // Check number of input arguments
    // =========================================================================
    rhs = argn(2);
    if rhs <> 0 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsLoadLoad",0));
    end

    // loaded_file
    // =========================================================================
    loaded_file = pathconvert(TMPDIR+"/atoms/loaded.bin",%F);

    if fileinfo(loaded_file) <> [] then
        load(loaded_file,"loaded_mat");
        matrix_out = loaded_mat;
    end

endfunction
