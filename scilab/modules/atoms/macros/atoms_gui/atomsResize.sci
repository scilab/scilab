// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
