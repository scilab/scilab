// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function atomsGui()

    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    // Test connection
    allModules = [];
    errStatus  = execstr("allModules = atomsDESCRIPTIONget();", "errcatch");

    if errStatus<>0 | size(allModules, "*") == 0 then
        if size(atomsRepositoryList(),"*") > 0 then
            messagebox(gettext("No ATOMS module is available. Please, check your Internet connection."), gettext("ATOMS error"), "error");
        else
            messagebox(gettext("No ATOMS module is available: your repository list is empty."), gettext("ATOMS error"), "error");
        end
        return
    end

    // Parameters
    // =========================================================================

    // Figure width & height
    figwidth         = 800;
    figheight        = 500;

    // Margin
    margin           = 10;
    widgetHeight     = 25;

    // Message Frame
    msgWidth         = figwidth -2*margin;
    msgHeight        = 30;

    // Button
    buttonHeight     = 20;

    // Font Size
    defaultFontSize  = 12;

    // Close the window if it already exists
    oldFig = findobj("tag", "atomsFigure");
    if ~isempty(oldFig) then
        delete(oldFig);
    end

    // Create the main window
    // =========================================================================

    atomsfig = figure( ..
        "figure_name", gettext("ATOMS"), ..
        "position"   , [0 0 figwidth figheight],..
        "background" , -2,..
        "UserData"   , allModules, ..
        "tag"        , "atomsFigure");

    // Remove Scilab graphics menus & toolbar
    // =========================================================================

    delmenu(atomsfig.figure_id, gettext("&File"));
    delmenu(atomsfig.figure_id, gettext("&Tools"));
    delmenu(atomsfig.figure_id, gettext("&Edit"));
    delmenu(atomsfig.figure_id, gettext("&?"));
    toolbar(atomsfig.figure_id, "off");

    // Add ATOMS Menu
    // =========================================================================

    // Menu File
    h = uimenu( ..
        "parent"   , atomsfig, ..
        "label"    , gettext("File"));

    // Menu File:Home
    uimenu( ..
        "parent"   , h, ..
        "label"    , gettext("Home"), ..
        "callback" , "cbAtomsGui", ..
        "tag"      , "homeMenu");

    // Menu File:Close
    uimenu( ..
        "parent"   , h, ..
        "label"    , gettext("Close"), ..
        "callback" , "cbAtomsGui", ..
        "tag"      , "closeMenu");

    // Menu ?
    h = uimenu( ..
        "parent"   , atomsfig, ..
        "label"    , gettext("?"));

    // Menu ?:Atoms Help...
    uimenu( ..
        "parent"   , h, ..
        "label"    , gettext("Atoms Help..."), ..
        "callback" , "cbAtomsGui", ..
        "tag"      , "helpMenu");

    // Build the module list (listbox on the left)
    // =========================================================================
    LeftElements = atomsGetLeftListboxElts("filter:main");

    // Build the installed module list
    // =========================================================================
    HomeElements = getHomeListboxElements();

    // Set the figure size ... after all delmenu(s)
    // =========================================================================
    atomsfig.axes_size = [figwidth figheight];

    // List of modules
    // =========================================================================

    listboxWidth              = 200;
    listboxFrameWidth         = listboxWidth + 2*margin;

    listboxFrameHeight        = figheight- 3*margin - msgHeight;
    listboxHeight             = listboxFrameHeight - 2*margin;

    // Figure name
    atomsfig("figure_name")   = LeftElements("title")+" - ATOMS";

    // Frame
    LeftFrame                 = uicontrol( ..
        "Parent"              , atomsfig,..
        "Style"               , "frame",..
        "Relief"              , "solid",..
        "Position"            , [margin widgetHeight+2*margin listboxFrameWidth listboxFrameHeight],..
        "Background"          , [1 1 1],..
        "UserData"            , "filter:main",..
        "Tag"                 , "LeftFrame");

    // Listbox
    LeftListbox               = uicontrol( ..
        "Parent"              , LeftFrame,..
        "Style"               , "listbox",..
        "Position"            , [ margin margin listboxWidth listboxHeight],..
        "Background"          , [1 1 1],..
        "FontSize"            , defaultFontSize,..
        "String"              , LeftElements("items_str"),..
        "UserData"            , LeftElements("items_mat"),..
        "Callback"            , "cbAtomsGui", ..
        "Min"                 , 1, ..
        "Max"                 , 1, ..
        "Tag"                 , "LeftListbox")

    // Description of a module
    // =========================================================================

    descFrameWidth           = figwidth - listboxFrameWidth - 3*margin;
    descFrameHeight          = listboxFrameHeight;

    descWidth                = descFrameWidth  - 2*margin;
    descHeight               = descFrameHeight - 4*margin - buttonHeight;

    // Frame
    DescFrame                = uicontrol( ..
        "Parent"             , atomsfig,..
        "Style"              , "frame",..
        "Relief"             , "solid",..
        "Background"         , [1 1 1],..
        "Position"           , [listboxFrameWidth+2*margin widgetHeight+2*margin descFrameWidth descFrameHeight],..
        "Tag"                , "DescFrame", ..
        "Visible"            , "off");

    // Frame title
    DescTitle                = uicontrol( ..
        "Parent"             , DescFrame,..
        "Style"              , "text",..
        "Position"           , [2*margin descFrameHeight-1.5*margin 200 widgetHeight],..
        "HorizontalAlignment", "center",..
        "VerticalAlignment"  , "middle",..
        "String"             , gettext("List of installed modules"), ..
        "FontWeight"         , "bold",..
        "FontSize"           , 12,..
        "Background"         , [1 1 1],..
        "Tag"                , "DescTitle");

    // Details of a module
    Desc                     = uicontrol( ..
        "Parent"             , DescFrame,..
        "VerticalAlignment"  , "top",..
        "Style"              , "text",..
        "Position"           , [ margin margin+buttonHeight+2*margin descWidth descHeight],..
        "Background"         , [1 1 1],..
        "FontSize"           , defaultFontSize,..
        "String"             , "",..
        "Tag"                , "Desc");

    // Buttons
    // -------------------------------------------------------------------------

    buttonWidth = (descFrameWidth - 4*margin) / 3;

    // "Remove" Button
    removeButton             = uicontrol( ..
        "Parent"             , DescFrame,..
        "Style"              , "pushbutton",..
        "Position"           , [margin margin buttonWidth widgetHeight],..
        "String"             , gettext("Remove"),..
        "Callback"           , "cbAtomsGui", ..
        "Enable"             , "off",..
        "Tag"                , "removeButton");

    // "Install" Button
    installButton            = uicontrol( ..
        "Parent"             , DescFrame,..
        "Style"              , "pushbutton",..
        "Position"           , [buttonWidth+2*margin margin buttonWidth widgetHeight],..
        "String"             , gettext("Install"),..
        "Callback"           , "cbAtomsGui", ..
        "Enable"             , "off", ..
        "Tag"                , "installButton");

    // "Update" Button
    updateButton             = uicontrol( ..
        "Parent"             , DescFrame,..
        "Style"              , "pushbutton",..
        "Position"           , [2*buttonWidth+3*margin margin buttonWidth widgetHeight],..
        "String"             , gettext("Update"),..
        "Callback"           , "cbAtomsGui", ..
        "Enable"             , "off", ..
        "Tag"                , "updateButton");

    // Home: List of installed modules
    // =========================================================================

    descWidth                = descFrameWidth  - 2*margin;
    descHeight               = descFrameHeight - 3*margin;

    // Frame
    HomeFrame                = uicontrol( ..
        "Parent"             , atomsfig,..
        "Style"              , "frame",..
        "Relief"             , "solid",..
        "Background"         , [1 1 1],..
        "Position"           , [listboxFrameWidth+2*margin widgetHeight+2*margin descFrameWidth descFrameHeight],..
        "Tag"                , "HomeFrame");

    // Frame title
    HomeTitle                = uicontrol( ..
        "Parent"             , HomeFrame,..
        "Style"              , "text",..
        "Position"           , [2*margin descFrameHeight-1.5*margin 200 widgetHeight],..
        "HorizontalAlignment", "center",..
        "VerticalAlignment"  , "middle",..
        "String"             , gettext("List of installed modules"), ..
        "FontWeight"         , "bold",..
        "FontSize"           , 12,..
        "Background"         , [1 1 1],..
        "Tag"                , "HomeTitle");

    // Home
    HomeListbox               = uicontrol( ..
        "Parent"              , HomeFrame,..
        "Style"               , "listbox",..
        "Position"            , [ margin margin descWidth descHeight],..
        "Background"          , [1 1 1],..
        "FontSize"            , defaultFontSize,..
        "String"              , HomeElements("items_str"),..
        "UserData"            , HomeElements("items_mat"),..
        "Callback"            , "cbAtomsGui", ..
        "Min"                 , 1, ..
        "Max"                 , 1, ..
        "Tag"                 , "HomeListbox");

    // Message Frame
    // =========================================================================

    // Frame
    msgFrame                 = uicontrol( ..
        "Parent"             , atomsfig,..
        "Style"              , "frame",..
        "Relief"             , "solid",..
        "Background"         , [1 1 1],..
        "Position"           , [margin margin msgWidth msgHeight],..
        "Tag"                , "msgFrame");

    // Text
    msgText                  = uicontrol( ..
        "Parent"             , msgFrame,...
        "Style"              , "text",..
        "HorizontalAlignment", "left",..
        "VerticalAlignment"  , "middle",..
        "String"             , "", ..
        "FontSize"           , 12,..
        "Background"         , [1 1 1],..
        "Position"           , [2 2 msgWidth-10 msgHeight-4],..
        "Tag"                , "msgText");

endfunction

// =============================================================================
// getHomeListboxElements()
//
// Returns a struct that contains the followings fields:
//  - elements("items_str")
//  - elements("items_mat")
//
// =============================================================================

function elements = getHomeListboxElements()

    items_str  = [];
    items_mat  = [];

    installed  = atomsGetInstalled();
    atomsfig   = findobj("tag","atomsFigure");
    allModules = atomsfig("UserData");


    for i=1:size(installed(:,1), "*")

        MRVersionAvailable = atomsGetMRVersion(installed(i,1));
        MRVersionInstalled = atomsVersionSort(atomsGetInstalledVers(installed(i,1)),"DESC");
        MRVersionInstalled = MRVersionInstalled(1);
        if atomsVersionCompare(MRVersionInstalled,MRVersionAvailable) == -1 then
            // Not up-to-date
            icon = "notuptodate.png";
        else
            // The Most Recent Version is already installed
            icon = "installed.png";
        end

        if modulo(i,2) == 0 then
            background = "#eeeeee";
        else
            background = "#ffffff";
        end

        thisItem =            "<html>";

        thisItem = thisItem + "<table style=""background-color:"+background+";color:#000000;"" ><tr>";
        thisItem = thisItem + "<td><img src=""file:///"+SCI+"/modules/atoms/images/icons/"+icon+""" /></td>";
        thisItem = thisItem + "<td>";
        thisItem = thisItem + "  <div style=""width:383px;text-align:left;"">";
        thisItem = thisItem + "    <span style=""font-weight:bold;"">"+allModules(installed(i,1))(installed(i,2)).Title+" "+installed(i,2)+"</span><br />";
        thisItem = thisItem + "    <span>"+allModules(installed(i,1))(installed(i,2)).Summary+"</span><br />";
        thisItem = thisItem + "    <span style=""font-style:italic;"">"+installed(i,4)+"</span>";
        thisItem = thisItem + "  </div>";
        thisItem = thisItem + "</td>";
        thisItem = thisItem + "</tr></table>";
        thisItem = thisItem + "</html>";

        items_str = [items_str ; thisItem];
        items_mat = [items_mat ; "module" installed(i,1)];
    end

    if items_str==[] then
        elements("items_str") = "";
    else
        elements("items_str") = items_str;
    end

    elements("items_mat") = items_mat;

endfunction
