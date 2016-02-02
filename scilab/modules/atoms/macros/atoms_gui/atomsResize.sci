// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function atomsResize()

    if ~isempty(get("HomeListbox")) then
    // Load Atoms Internals lib if it's not already loaded
        if ~exists("atomsinternalslib") then
            load("SCI/modules/atoms/macros/atoms_internals/lib");
        end

        HomeElements = atomsGetHomeListboxElts();
        if ~isempty(get("HomeListbox")) then
            set("HomeListbox", "String", HomeElements("items_str"));
            set("HomeListbox", "UserData", HomeElements("items_mat"));
        end
    end
endfunction
