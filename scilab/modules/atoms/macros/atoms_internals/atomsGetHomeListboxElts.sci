// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2009-2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2013 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function elements = atomsGetHomeListboxElts()
    items_str  = [];
    items_mat  = [];

    installed  = atomsGetInstalled();
    tmp = atomsAutoloadList("all")
    autoloaded = tmp(:,1)
    if ~isempty(get("atomsFigure")) then
        allModules = get("atomsFigure", "UserData");

        items_str  = atomsSetInstalledList(installed)
        items_mat = installed(:,1)
        items_mat = [emptystr(items_mat)+"module" items_mat ]

        if items_str==[] then
            elements("items_str") = "";
        else
            elements("items_str") = items_str;
        end

        elements("items_mat") = items_mat;
    else
        elements = [];
    end
endfunction
