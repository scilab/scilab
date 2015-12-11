// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// =============================================================================
//
// items_str = atomsSetInstalledList(installed)
//
// items_str must be updated: it contains infos about the autoload status
//  of each installed module, while this status can be interactively switched
// =============================================================================

function items_str = atomsSetInstalledList(installed)

    // Load Atoms Internals lib if it's not already loaded
    if ~exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    tmp = atomsAutoloadList("all")
    autoloaded = tmp(:,1)
    allModules = get("atomsFigure", "UserData");

    // Widths of HTML columns
    id = get("LayerFrame")
    pos = id.position - 10;
    col1Width = max([315 pos(3)*0.8]); // short description
    col2Width = max([60 pos(3)*0.2]);  // autoload status

    items_str = []
    for i = 1:size(installed(:,1),"*")
        MRVersionAvailable = atomsGetMRVersion(installed(i,1));
        MRVersionInstalled = atomsVersionSort(atomsGetInstalledVers(installed(i,1)),"DESC");
        MRVersionInstalled = MRVersionInstalled(1);
        if atomsVersionCompare(MRVersionInstalled,MRVersionAvailable) == -1 then
            // Not up-to-date
            icon = "software-update-notinstalled.png";
        else
            // The Most Recent Version is already installed
            icon = "software-update-installed.png";
        end

        if modulo(i,2) == 0 then
            background = "#eeeeee";
        else
            background = "#ffffff";
        end

        // Building the content
        if or(installed(i,1)==autoloaded) then
            tmp = "<span style=""color:green"">"+_("Loaded<br>at startup")+"</span>"
        else
            tmp = ""
        end

        thisItem = [
        "<html>"
        "<table style=""background-color:"+background+";color:#000000;"">"
        "<tr>"
        "<td>"
        "<img src=""file:///"+SCI+"/modules/atoms/images/icons/16x16/status/"+icon+""" />"
        "</td>"
        "<td><div style=""width:"+sprintf("%d",col1Width*0.72)+"px;text-align:left;"">"
        "<span style=""font-weight:bold;"">"
        allModules(installed(i,1))(installed(i,2)).Title+" "
        installed(i,2)
        "</span>"
        "<br/>"
        "<span>"+allModules(installed(i,1))(installed(i,2)).Summary+"</span>"
        "<br/>"
        "<span style=""font-style:italic;"">"+installed(i,4)+"</span>"
        "</div>"
        "</td>"
        "<td style=""text-align:center;width:"+sprintf("%d",col2Width*0.7)+"px"">"
        tmp
        "</td>"
        "</tr>"
        "</table>"
        "</html>"
        ]

        items_str = [ items_str ; strcat(thisItem) ];
    end
    if items_str==[] then
        items_str = ""
    end
endfunction
