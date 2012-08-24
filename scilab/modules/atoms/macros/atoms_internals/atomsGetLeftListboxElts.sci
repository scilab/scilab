// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function
// GUI function

// Returns a struct that contains the followings fields:
//  - elements("items_str")
//  - elements("items_mat")
//  - elements("title")
//

function elements = atomsGetLeftListboxElts(category)

    rhs = argn(2);

    // Check number of input arguments
    // =========================================================================

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"getLeftListboxElements",1));
    end

    // Check input argument types
    // =========================================================================

    if category<>[] & type(category) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"getLeftListboxElements",1));
    end

    if category<>[] & type(category) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"getLeftListboxElements",1));
    end

    if size(category(1,:),"*") > 2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1 or mx2 string matrix expected.\n"),"atomsIsCategory",1));
    end

    // Build lists
    // =========================================================================

    items_str  = [];
    items_mat  = [];

    atomsfig   = findobj("tag","atomsFigure");
    allModules = atomsfig("UserData");

    background = "#ffffff";

    // 1st case, All main categories
    // -------------------------------------------------------------------------

    if category=="filter:main" then

        categories = atomsCategoryGet("filter:main");

        thisItem  =            "<html>";
        thisItem  = thisItem + "<table style=""background-color:#ffffff;color:#000000;""><tr>";
        thisItem  = thisItem + "<td><img src=""file:///"+SCI+"/modules/gui/images/icons/16x16/places/user-home.png"" /></td>";
        thisItem  = thisItem + "<td style=""width:132px;""><div style=""width:118px;text-align:left;font-weight:bold;"">"+gettext("All modules")+"</div></td>";
        thisItem  = thisItem + "</tr></table>";
        thisItem  = thisItem + "</html>";

        items_str = [items_str ; thisItem ];
        items_mat = [items_mat ; "category" "filter:all"];

         for i=1:size(categories, "*")

            if background == "#eeeeee" then
                background = "#ffffff";
            else
                background = "#eeeeee";
            end

            thisItem  =            "<html>";
            thisItem  = thisItem + "<table style=""background-color:"+background+";color:#000000;"" ><tr>";
            thisItem  = thisItem + "<td><img src=""file:///"+SCI+"/modules/gui/images/icons/16x16/places/folder.png"" /></td>";
            thisItem  = thisItem + "<td style=""width:132px;""><div style=""width:118px;text-align:left;"">"+categories(i)+"</div></td>";
            thisItem  = thisItem + "</tr></table>";
            thisItem  = thisItem + "</html>";

            items_str = [items_str ; thisItem ];
            items_mat = [items_mat ; "category" categories(i)];
        end

    // 2nd case: Sub categories + modules
    // -------------------------------------------------------------------------

    else

        // Categories
        // ---------------------------------------------------------------------

        index_start = regexp(category,"/ - /");
        // if index_start == [], category is a main category
        // otherwise, category is a subcategory

        if category=="filter:all" then
            categories = [];
            top_str    = gettext("Main categories");
            top_mat    = ["category" "filter:main"];

        elseif index_start==[] then
            categories = atomsCategoryGet(category);
            top_str    = gettext("Main categories");
            top_mat    = ["category" "filter:main"];

        else
            categories = [];
            parent     = part(category,1:index_start-1);
            top_str    = parent;
            top_mat    = ["category" parent];

        end

        thisItem  =            "<html>";
        thisItem  = thisItem + "<table style=""background-color:#ffffff;color:#000000;"" ><tr>";
        thisItem  = thisItem + "<td><img src=""file:///"+SCI+"/modules/gui/images/icons/16x16/places/user-home.png"" /></td>";
        thisItem  = thisItem + "<td style=""width:132px;""><div style=""width:118px;text-align:left;font-weight:bold;"">"+top_str+"</div></td>";

        thisItem  = thisItem + "</tr></table>";
        thisItem  = thisItem + "</html>";

        items_str = [items_str ; thisItem ];
        items_mat = [items_mat ; top_mat  ];

        for i=1:size(categories, "*")

            if background == "#eeeeee" then
                background = "#ffffff";
            else
                background = "#eeeeee";
            end

            thisItem  =            "<html>";
            thisItem  = thisItem + "<table style=""background-color:"+background+";color:#000000;"" ><tr>";
            thisItem  = thisItem + "<td><img src=""file:///"+SCI+"/modules/gui/images/icons/16x16/places/folder.png"" /></td>";
            thisItem  = thisItem + "<td style=""width:132px;""><div style=""width:118px;text-align:left;"">"+categories(i)+"</div></td>";
            thisItem  = thisItem + "</tr></table>";
            thisItem  = thisItem + "</html>";

            items_str = [items_str ; thisItem ];
            items_mat = [items_mat ; "category" category+" - "+categories(i) ];
        end

        // Modules
        // ---------------------------------------------------------------------

        if category=="filter:all" then
            modulesNames = atomsGetAvailable([],%T);
        else
            modulesNames = atomsGetAvailable(category,%T);
        end

        MRVersionAvailable = atomsGetMRVersion(modulesNames);
        modulesIsInstalled = atomsIsInstalled(modulesNames);

        for i=1:size(modulesNames, "*")

            thisModuleTitle    = allModules(modulesNames(i))(MRVersionAvailable(i)).Title;

            if modulesIsInstalled(i) then

                MRVersionInstalled = atomsVersionSort(atomsGetInstalledVers(modulesNames(i)),"DESC");
                MRVersionInstalled = MRVersionInstalled(1);
                thisModuleTitle    = allModules(modulesNames(i))(MRVersionInstalled).Title;

                if atomsVersionCompare(MRVersionInstalled,MRVersionAvailable(i)) == -1 then
                    // Not up-to-date
                    icon = "software-update-notuptodate.png";
                else
                    // The Most Recent Version is already installed
                    icon = "software-update-installed.png";
                end

            else
                icon = "software-update-notinstalled.png";
            end

            if background == "#eeeeee" then
                background = "#ffffff";
            else
                background = "#eeeeee";
            end

            thisItem =            "<html>";
            thisItem = thisItem + "<table style=""background-color:"+background+";color:#000000;"" ><tr>";
            thisItem = thisItem + "<td><img src=""file:///"+SCI+"/modules/atoms/images/icons/16x16/status/"+icon+""" /></td>";
            thisItem = thisItem + "<td style=""width:132px;""><div style=""width:118px;text-align:left;"">"+thisModuleTitle+"</div></td>";
            thisItem = thisItem + "</tr></table>";
            thisItem = thisItem + "</html>";

            items_str = [items_str ; thisItem     ];
            items_mat = [items_mat ; "module" modulesNames(i)];

        end

    end

    if items_str==[] then
        elements("items_str") = "";
    else
        elements("items_str") = items_str;
    end

    elements("items_mat") = items_mat;

    if category=="filter:main" then
        elements("title") = gettext("Main categories");
    elseif category=="filter:all" then
        elements("title") = gettext("All modules");
    else
        elements("title") = category;
    end

endfunction
