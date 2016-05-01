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

// End user function

// Return a 5xn string matrix :
//
// !toolbox2          0.1  user     /home/pmarecha/.Scilab/scilab-branch-atoms/atoms/contrib/toolbox2/0.1  A  !
// !                                                                                                          !
// !toolbox2          0.2  user     /home/pmarecha/.Scilab/scilab-branch-atoms/atoms/contrib/toolbox2/0.2  A  !
// !                                                                                                          !
// !toolbox_skeleton  1.3  alluser  /home/pmarecha/work/atoms/scilab/contrib/toolbox_skeleton/1.3          I  !

function packages = atomsGetInstalled(section)

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    rhs      = argn(2);
    packages = [];

    // Check number of input arguments
    // =========================================================================

    if rhs > 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsGetInstalled",1));
    end

    // Load all packages, "user" section or "allusers" section packages ?
    // =========================================================================

    if rhs == 0 then
        section = "all";
    else

        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsGetInstalled",1));
        end

        if and(section<>["user","allusers","all"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'',''allusers'' or ''all'' expected.\n"),"atomsGetInstalled",1));
        end

    end

    // Call atomsLoadInstalledMat
    // =========================================================================
    packages = atomsLoadInstalledMat(section);

endfunction
