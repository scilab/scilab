// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2009 - Allan CORNET
// Copyright (C) 2012 - 2015 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//===========================================================
function [y] = toolboxes(path)
    // INTERNAL macro should NOT used by users
    // toolboxes loading
    // path is a directory to explore for contribs
    //===========================================================
    global %toolboxes
    global %toolboxes_dir
    //===========================================================
    [lhs,rhs] = argn(0)
    y = []
    // Action
    if (rhs == 1) & typeof(path)=="constant" then
        // return string to exec
        tmp = %toolboxes(path);
        if part(tmp,1)=="!" then   // ATOMS module
            atomsMod = atomsGetInstalled();
            tmp = strtok(tmp, ",") // Trims the part of the release numbers
            name = atomsMod(find(atomsMod(:,1)==part(tmp, 2:length(tmp))),1);
            y = "atomsLoad("""+name+""")"
        else
            Path = %toolboxes_dir + stripblanks(tmp);
            y = "exec(""" + pathconvert(Path) + filesep() + "loader.sce" + """);"
        end
        return
    end

    // Non ATOMS modules
    if rhs == 0 then
        path = SCI + filesep() + "contrib";
    end

    cur_wd = pwd();
    chdir(path);
    files = listfiles(".");
    contribs = [];
    for k = 1:size(files,"*")
        if isfile(files(k)+"/loader.sce") then
            contribs = [contribs ; files(k)];
        end
    end

    // ATOMS modules without autoloading
    installed   = atomsGetInstalled()
    autoloading = atomsAutoloadList()
    for i = 1:size(installed,1)
        if and(installed(i,1)~=autoloading(:,1)) then
            contribs = [contribs ; "!"+installed(i,1)+","+installed(i,2)]
        end
    end

    // Creating the menu and its entries
    if (contribs <> []) & (getscilabmode() == "STD") then
        contribs = gsort(contribs, "g", "i")
        delmenu(gettext("&Toolboxes"));
        h = uimenu("parent", 0, "label", gettext("&Toolboxes"));
        for k=1:size(contribs,"*")
            ptitle = strsubst(contribs(k),","," ");
            if part(ptitle,1)=="!"
                ptitle = part(ptitle,2:$)+"  (atoms)";
            end
            m = uimenu(h,"label", ptitle, ..
            "callback","execstr(toolboxes("+msprintf("%d",k)+"))");
        end
        unsetmenu(gettext("&Toolboxes"));
    end

    %toolboxes = contribs;
    %toolboxes_dir = pathconvert(path);
    chdir(cur_wd);

endfunction
//===========================================================
