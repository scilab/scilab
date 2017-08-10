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

// get the list of repositories

function modules = atomsAutoloadList(section)

    // Load Atoms Internals lib if it's not already loaded
    // =========================================================================
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    rhs      = argn(2);
    modules  = [];

    // Check number of input arguments
    // =========================================================================

    if rhs > 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsAutoloadList",1));
    end

    // Allusers/user management
    //   - If Allusers is equal to "all", display both "user" and "allusers" autoload list
    //       → SCI/.atoms/autoloaded
    //       → SCIHOME/atoms/autoloaded
    //   - If Allusers is equal to "allusers",display only "allusers" autoload list
    //       → SCI/.atoms/autoloaded
    //   - If Allusers is equal to "user",display only "user" autoload list
    //       → SCIHOME/atoms/autoloaded
    // =========================================================================

    if rhs < 1 then
        section = "all";

    else

        if type(section) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsAutoloadList",1));
        end

        if size(section,"*")<>1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsAutoloadList",1));
        end

        if and(section<>["user","allusers","all"]) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'',''allusers'' or ''all'' expected.\n"),"atomsAutoloadList",1));
        end

    end

    if section == "all" then
        sections = ["user";"allusers"];
    else
        sections = section;
    end

    // Loop on sections
    // =========================================================================

    for i=1:size(sections,"*")

        autoloaded = atomsAutoloadLoad(sections(i));

        if ~ isempty(autoloaded) then
            modules = [ modules ; autoloaded emptystr(size(autoloaded(:,1),"*"),1) + sections(i) ];
        end

    end

endfunction
