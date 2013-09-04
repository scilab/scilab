// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
