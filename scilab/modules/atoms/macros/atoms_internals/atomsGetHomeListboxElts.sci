// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2009-2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
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

function elements = atomsGetHomeListboxElts()
    items_str  = "";
    items_mat  = "";

    installed  = atomsGetInstalled();
    tmp = atomsAutoloadList("all")
    autoloaded = tmp(:,1)
    if ~isempty(get("atomsFigure")) then
        allModules = get("atomsFigure", "UserData");

        items_str  = atomsSetInstalledList(installed)
        items_mat = installed(:,1)
        if isempty(items_str)
            items_str = "";
            items_mat = "";
        end
        items_mat = [emptystr(items_mat)+"module" items_mat ]

        elements("items_str") = items_str;
        elements("items_mat") = items_mat;
    else
        elements = [];
    end
endfunction
