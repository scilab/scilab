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

// display of the available toolboxes

function packages = atomsOrphanList(section)

    packages = [];

    // Check input parameters
    // =========================================================================

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"atomsOrphanList",1))
    end

    // Check input argument type
    // =========================================================================

    if type(section) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsOrphanList",1));
    end

    if and(section<>["user","allusers","all"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' or ''all'' expected.\n"),"atomsOrphanList",1));
    end

    // Loop on installed packages
    // =========================================================================

    installed_list = atomsGetInstalled(section);

    for i=1:size(installed_list(:,1),"*")
        if isempty( atomsGetDepParents([installed_list(i,1) installed_list(i,2)] , section )) ..
            & (atomsGetInstalledStatus([installed_list(i,1) installed_list(i,2)],section) == "A") then
            packages = [ packages ; installed_list(i,:) ];
        end
    end

endfunction
