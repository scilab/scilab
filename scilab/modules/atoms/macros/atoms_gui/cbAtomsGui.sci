// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2009-2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function cbAtomsGui()
    // Load Atoms Internals lib if it's not already loaded
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    UItag = get(gcbo,"Tag");

    // Display selected module information
    // =========================================================================
    if or(UItag == ["LeftListbox";"HomeListbox"]) then

        if get(UItag, "string") == "" then
            return;
        end
        // Get the selected module
        selected = getSelected(UItag);
        if selected(1) == "module" then
            // Save the module name
            set("DescFrame", "userdata", selected(2));
            // Update the description frame
            updateDescFrame();
            // Show the description frame
            showDesc();
        elseif selected(1)=="category" then
            LeftElements = atomsGetLeftListboxElts(selected(2));
            set("LeftListbox", "String", LeftElements("items_str"));
            set("LeftListbox", "UserData", LeftElements("items_mat"));
            set("LeftFrame", "UserData", selected(2));

            // Figure name
            set("atomsFigure", "figure_name", LeftElements("title")+" - ATOMS");
        end
    end

    // A button has been pressed
    // =========================================================================
    if or(UItag == ["installButton";"removeButton"]) then
        // Get the module name
        module = get("DescFrame","userdata");
        // Disable callbacks
        disableAtomsGui();
    end

    // Install or Update selected module
    // =========================================================================
    if UItag == "installButton" then
        if get("installButton", "String") == _("Install") then
            updateStatusBar("info",_("Installing") + "...");
            if execstr("atomsInstall("""+module+""")","errcatch")<>0 then
                updateStatusBar();
                messagebox(_("Installation failed!"),_("ATOMS error"),"error");
            else
                updateDescFrame();
                updateStatusBar("success",_("Installation done! Please restart Scilab to take changes into account."));
            end
        else
            updateStatusBar("info",_("Updating") + "...");
            if execstr("atomsUpdate("""+module+""")","errcatch")<>0 then
                updateStatusBar();
                messagebox(_("Update failed!"),_("ATOMS error"),"error");
            else
                updateDescFrame();
                updateStatusBar("success",_("Update done! Please restart Scilab to take changes into account."));
            end
        end
    // Remove selected module
    // =========================================================================
    elseif UItag == "removeButton" then // Remove selected module
        updateStatusBar("info",_("Removing")+" ...");
        if execstr("atomsRemove("""+module+""")", "errcatch")<>0 then
            updateStatusBar();
            messagebox(_("Remove failed!"),_("ATOMS error"),"error");
        else
            updateDescFrame();
            updateStatusBar("success",_("Remove done! Please restart Scilab to take changes into account. "));
        end
    end

    if UItag == "autoloadCheck" then
        module = get("DescFrame", "userdata")
        if get("autoloadCheck", "value") == get("autoloadCheck", "max") then
            atomsAutoloadAdd(module)
            msg = _("The module will be automatically loaded at next startup.")
            updateStatusBar("info", msg)
        else
            atomsAutoloadDel(module)
            msg = _("Autoload at startup is canceled. The ""Toolboxes"" menu or atomsLoad() can be used to load the module when needed.")
            updateStatusBar("info", msg)
        end
    end

    // End of the button action
    // =========================================================================
    if or(UItag == ["installButton";"removeButton"]) then
        enableLeftListbox();
        reloadLeftListbox();
    end

    // Menu
    // =========================================================================
    // File:Home
    if or(UItag == ["homeMenu";"backButton"]) then
        showHome();
    // File:Close
    elseif UItag == "closeMenu" then
        delete(findobj("Tag", "atomsFigure"));
    // ?:Help
    elseif UItag == "helpMenu" then
        help("atoms")
    end
endfunction

// =============================================================================
// getSelected()
//  + Return the type: category / module
//  + Return the name selected from a listbox.
// =============================================================================
function selected = getSelected(listbox)
    index    = get(listbox, "Value");
    UserData = get(listbox, "UserData");
    selected = UserData(index,:);
endfunction

// =============================================================================
// disableAtomsGui()
//  + Disable all callback
// =============================================================================
function disableAtomsGui()
    set("installButton", "Enable", "off");
    set("removeButton", "Enable", "off");
    disableLeftListbox()
endfunction

function disableLeftListbox()
    set("LeftListbox", "Callback", "");
    set("LeftListbox", "ForegroundColor", [0.5 0.5 0.5]);
endfunction

function enableLeftListbox()
    set("LeftListbox", "Callback", "cbAtomsGui");
    set("LeftListbox", "ForegroundColor", [0 0 0]);
endfunction

// =============================================================================
// reloadLeftListbox()
// =============================================================================
function reloadLeftListbox()
    category = get("LeftFrame", "UserData");
    LeftElements = atomsGetLeftListboxElts(category);
    set("LeftListbox", "String", LeftElements("items_str"));
    set("LeftListbox", "UserData", LeftElements("items_mat"));
endfunction

// =============================================================================
// updateDescFrame()
//  + Update the description frame with the selected module
//  + does not change the description frame visibility
// =============================================================================
function updateDescFrame()
    // Operating system detection + Architecture detection
    // =========================================================================
    [OSNAME,ARCH,LINUX,MACOSX,SOLARIS,BSD] = atomsGetPlatform();

    // Get the modules list and the selected module
    // =========================================================================
    allModules     = get("atomsFigure", "userdata");
    thisModuleName = get("DescFrame" ,"userdata");

    // Reset the message frame
    // =========================================================================
    updateStatusBar();

    // Get the module details
    // =========================================================================
    modulesNames       = getfield(1, allModules);
    modulesNames (1:2) = [];
    thisModuleStruct   = allModules(thisModuleName);

    MRVersionAvailable = atomsGetMRVersion(thisModuleName);
    MRVersionInstalled = "";

    if atomsIsInstalled(thisModuleName) then
        MRVersionInstalled = atomsVersionSort(atomsGetInstalledVers(thisModuleName),"DESC");
        MRVersionInstalled = MRVersionInstalled(1);
        thisModuleDetails  = thisModuleStruct(MRVersionInstalled);
    else
        thisModuleDetails  = thisModuleStruct(MRVersionAvailable);
    end

    // Download Size
    // =========================================================================
    sizeHTML = "";

    if isfield(thisModuleDetails,OSNAME+ARCH+"Size") then
        sizeHTML = txt2title(_("Download size")) ..
        + "<div>" ..
        + atomsSize2human(thisModuleDetails(OSNAME+ARCH+"Size")) ..
        + "</div>";
    end

    // Authors
    // =========================================================================
    authorMat  = thisModuleDetails.Author;
    authorHTML = "";

    for i=1:size(authorMat,"*")
        authorHTML = authorHTML + authorMat(i)+"<br>";
    end

    authorHTML = txt2title(_("Author(s)")) ..
    + "<div>" ..
    + authorHTML
    + "</div>";

    // URLs (See also)
    // =========================================================================
    URLs        = [];
    seeAlsoHTML = "";

    if isfield(thisModuleDetails,"URL") & (thisModuleDetails.URL<>"") then
        URLs = [ URLs ; thisModuleDetails.URL ];
    end

    if isfield(thisModuleDetails,"WebSite") & (thisModuleDetails.WebSite<>"") then
        URLs = [ URLs ; thisModuleDetails.WebSite ];
    end

    if ~isempty(URLs) then

        for i=1:size(URLs,"*")
            seeAlsoHTML = seeAlsoHTML + "&nbsp;&bull;&nbsp;"+URLs(i)+"<br>";
        end

        seeAlsoHTML = txt2title(_("See also"))..
        + "<div>" ..
        + seeAlsoHTML ..
        + "</div>";
    end

    // Release date
    // =========================================================================
    dateHTML = "";

    if isfield(thisModuleDetails,"Date") ..
        & ~isempty(regexp(thisModuleDetails.Date,"/^[0-9]{4}-[0-1][0-9]-[0-3][0-9]\s/")) then

        dateHTML = txt2title(_("Release date")) ..
        + "<div>" ..
        + part(thisModuleDetails.Date,1:10) ..
        + "</div>";
    end

    // Build and Set the HTML code
    // =========================================================================
    htmlcode = "<html>" + ..
    "<body>" + ..
    txt2title(_("Version")) + ..
    "<div>" + thisModuleDetails.Version  + "</div>" + ..
    authorHTML + ..
    txt2title(_("Description")) + ..
    "<div>" + ..
    strcat(thisModuleDetails.Description,"<br>")  + ..
    "</div>" + ..
    seeAlsoHTML + ..
    dateHTML + ..
    sizeHTML + ..
    "</body>" + ..
    "</html>";

    // Process URLs and Emails
    htmlcode = processHTMLLinks(htmlcode);

    // Update the main description
    set("Desc", "String", htmlcode);

    // Description title management
    // =========================================================================
    descFrameHTML = thisModuleDetails.Title;

    border = get("DescFrame", "border");
    border.title = descFrameHTML;
    set("DescFrame", "border", border);

    // Buttons
    // =========================================================================
    // Tests for update available
    set("installButton", "String", _("Install"), "Enable", "off");
    set("removeButton", "Enable", "off");
    set("autoloadCheck", "Value", get("autoloadCheck", "min"), "Enable", "off");

    if atomsIsInstalled(thisModuleName) then
        if atomsVersionCompare(MRVersionInstalled,MRVersionAvailable) == -1 then
            // Not up-to-date
            set("installButton", "String", _("Update"), "Enable", "on");
            updateStatusBar("warning",sprintf(_("A new version (''%s'') of ''%s'' is available"),MRVersionAvailable,thisModuleDetails.Title));
        end

        set("removeButton", "Enable", "on");

        // Is autoloaded
        // -------------
        tmp = atomsAutoloadList()
        if or(thisModuleName==tmp) then
            set("autoloadCheck", "Value", get("autoloadCheck", "max"), "Enable", "on");
        else
            set("autoloadCheck", "Value", get("autoloadCheck", "min"), "Enable", "on");
        end
    else
        set("installButton", "String", _("Install"), "Enable", "on");
    end
endfunction

// =============================================================================
// atomsSize2human()
// =============================================================================
function human_str = atomsSize2human(size_str)
    size_int = strtod(size_str);
    if size_int < 1024 then
        human_str = string(size_int) + " " + _("Bytes");
    elseif size_int < 1024*1024 then
        human_str = string(round(size_int/1024)) + " " + _("KB");
    else
        human_str = string( round((size_int*10)/(1024*1024)) / 10 ) + " " + _("MB");
    end
endfunction

// =============================================================================
// show()
// =============================================================================
function show(tag)
    set(tag, "Visible", "On");
endfunction

// =============================================================================
// hide()
// =============================================================================
function hide(tag)
    set(tag, "Visible", "Off");
endfunction

// =============================================================================
// showHome()
// + Hide the detailed description of a module
// + Show the home page
// =============================================================================
function showHome()
    // Reset the message frame
    updateStatusBar();

    //refresh installed listbox
    HomeElements = atomsGetHomeListboxElts();
    set("HomeListbox", "String", HomeElements("items_str"));
    set("HomeListbox", "UserData", HomeElements("items_mat"));

    //active home layer
    set("LayerFrame", "String", "HomeFrame");
    //reset listbox selection
    set("HomeListbox", "value", []);

    // reset the left listbox
    LeftElements = atomsGetLeftListboxElts("filter:main");
    set("LeftListbox", "String", LeftElements("items_str"));
    set("LeftListbox", "UserData", LeftElements("items_mat"));
endfunction

// =============================================================================
// showDesc()
// + Hide the home page
// + Show the detailed description of a module
// =============================================================================
function showDesc()
    //active description layer
    set("LayerFrame", "String", "DescFrame");
endfunction

// =============================================================================
// updateStatusBar
// + Update the string in the msg Frame
// =============================================================================
function updateStatusBar(status,msg)
    rhs = argn(2);

    if rhs==0 then
        set("msgText", "String", "", "Icon", "");
        return
    end

    select status
    case "warning" then
        fontcolor = [0.75 0 0]; // red
        icon      = "software-update-available";
    case "success" then
        fontcolor = [0 0.5 0];// dark green
        icon      = "emblem-default";
    case "info" then
        fontcolor = [0.5 0.5 0.5]; // dark gray
        icon      = "dialog-information";
    end

    set("msgText", "Foregroundcolor", fontcolor,  "String", msg, "Icon", icon);
endfunction

// =============================================================================
// processHTMLLinks
// + Find URLs
// + Convert them in HTML (hyperlinks)
// =============================================================================
function txtout = processHTMLLinks(txtin)
    regexUrl   = "/((((H|h)(T|t)|(F|f))(T|t)(P|p)((S|s)?))\:\/\/)(www|[a-zA-Z0-9])[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,6}(\:[0-9]{1,5})*(\/($|[a-zA-Z0-9\.\,\;\?\''\\\+&amp;%\$#\=~_\-\/]+))*/";
    txtout     = "";

    [mat_start,mat_end,mat_match] = regexp(txtin,regexUrl);

    if ~isempty(mat_match) then
        mat_end = [ 0 mat_end ];
        for i=1:size(mat_match,"*")
            txtout = txtout + part(txtin,[mat_end(i)+1:mat_start(i)-1]) ..
            + "<a href="""+mat_match(i)+""" target=""_blank"">" ..
            + mat_match(i) ..
            + "</a>";
        end
        txtout = txtout + part(txtin,mat_end(size(mat_end,"*"))+1:length(txtin));
    else
        txtout = txtin;
    end
endfunction

// =============================================================================
// txt2title
// =============================================================================
function txtout = txt2title(txtin)
    txtout = "<div style=""font-weight:bold;margin-top:10px;margin-bottom:3px;"">" + ..
    txtin + ..
    "</div>";
endfunction

