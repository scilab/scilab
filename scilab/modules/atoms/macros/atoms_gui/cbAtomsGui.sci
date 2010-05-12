// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2009-2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

function cbAtomsGui()

    // Load Atoms Internals lib if it's not already loaded
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end

    UItag = get(gcbo,"Tag");

    // Get the description frame object
    DescFrame = findobj("tag","DescFrame");


    // Display selected module informations
    // =========================================================================

    if or(UItag == ["LeftListbox";"HomeListbox"]) then

        // Get the selected module
        module = getSelected(UItag);

        // Save the module name
        set(DescFrame,"userdata",module);

        // Update the description frame
        updateDescFrame();

        // Show the description frame
        showDesc();
    end

    // A button has been pressed
    // =========================================================================

    if or(UItag == ["installButton";"removeButton";"updateButton"]) then
        // Get the module name
        module = get(DescFrame,"userdata");

        // Disable callbacks
        disableAtomsGui();
    end

    // Install selected module
    // =========================================================================

    if UItag == "installButton" then
        if execstr("atomsInstall("""+module+""")","errcatch")<>0 then
            messagebox(gettext("Installation failed!"),gettext("Atoms error"),"error");
        else
            messagebox(gettext("Installation done! Please restart Scilab to take changes into account."),gettext("Atoms"),"info");
        end

    // Remove selected module
    // =========================================================================

    elseif UItag == "removeButton" then // Remove selected module
        if execstr("atomsRemove("""+module+""")", "errcatch")<>0 then
            messagebox(gettext("Remove failed!"),gettext("Atoms error"),"error");
        else
            messagebox(gettext("Remove done! Please restart Scilab to take changes into account. "),gettext("Atoms"),"info");
        end

    // Update selected module
    // =========================================================================

    elseif UItag == "updateButton" then // Update selected module
        if execstr("atomsUpdate("""+module+""")","errcatch")<>0 then
            messagebox(gettext("Update failed!"),gettext("Atoms error"),"error");
        else
            messagebox(gettext("Update done! Please restart Scilab to take changes into account."),gettext("Atoms"),"info");
        end
    end

    // End of the button action
    // =========================================================================

    if or(UItag == ["installButton";"removeButton";"updateButton"]) then
        updateDescFrame();
        enableLeftListbox();
    end


    // Menu
    // =========================================================================

    // File:Home
    if UItag == "homeMenu" then
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
//  + Return the module name selected from a listbox.
//  + Accepted listboxes:
//    - LeftListbox
//    - HomeListbox
// =============================================================================

function name = getSelected(listbox)

    selected = get(findobj("Tag",listbox),"Value");

    if listbox == "HomeListbox" then
        modulesNames       = atomsGetInstalled();
        modulesNames       = modulesNames(:,1);
    elseif listbox == "LeftListbox" then
        allModules         = get(findobj("Tag", "atomsFigure"), "userdata");
        modulesNames       = getfield(1, allModules);
        modulesNames (1:2) = [];
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"),"getSelected",1,"HomeListbox","LeftListbox"));
    end

    name = modulesNames(selected);

endfunction


// =============================================================================
// disableAtomsGui()
//  + Disable all callback
// =============================================================================


function disableAtomsGui()

    set(findobj("tag", "installButton"), "Enable", "off");
    set(findobj("tag", "updateButton") , "Enable", "off");
    set(findobj("tag", "removeButton") , "Enable", "off");
    disableLeftListbox()
endfunction

function disableLeftListbox()
    set(findobj("tag", "LeftListbox"),"Callback", "");
    set(findobj("tag", "LeftListbox"),"ForegroundColor",[0.5 0.5 0.5]);
endfunction

function enableLeftListbox()
    set(findobj("tag", "LeftListbox"),"Callback", "cbAtomsGui");
    set(findobj("tag", "LeftListbox"),"ForegroundColor",[0 0 0]);
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

    thisFigure     = findobj("tag","atomsFigure");

    Desc           = findobj("tag","Desc");
    DescFrame      = findobj("tag","DescFrame");
    DescTitle      = findobj("tag","DescTitle");

    allModules     = get(thisFigure,"userdata");
    thisModuleName = get(DescFrame ,"userdata");

    // Get the module details
    // =========================================================================

    modulesNames       = getfield(1, allModules);
    modulesNames (1:2) = [];
    thisModuleStruct   = allModules(thisModuleName);
    thisModuleVersions = getfield(1,thisModuleStruct);
    thisModuleDetails  = thisModuleStruct(thisModuleVersions(3));

    // Manage size
    // =========================================================================

    if isfield(thisModuleDetails,OSNAME+ARCH+"Size") then
        sizeHTML = ..
            "<div style=""font-weight:bold;margin-top:10px;margin-bottom:5px;"">" + ..
            gettext("Download size")                                              + ..
            "</div>"                                                              + ..
            "<div>"                                                               + ..
            atomsSize2human(thisModuleDetails(OSNAME+ARCH+"Size"))                + ..
            "</div>";
    else
        sizeHTML = "";
    end

    // Manage authors
    // =========================================================================

    authorMat  = thisModuleDetails.Author;

    authorHTML = "<div style=""font-weight:bold;margin-top:10px;margin-bottom:5px;"">" + ..
                 gettext("Author(s)") + ..
                 "</div>" + ..
                 "<div>";

    for i=1:size(authorMat,"*")
        authorHTML = authorHTML + authorMat(i)+"<br>";
    end

    authorHTML = authorHTML + "</div>";

    // Build and Set the HTML code
    // =========================================================================


    htmlcode = "<html>" + ..
               "<body>" + ..
               "<div style=""font-weight:bold;margin-top:10px;margin-bottom:5px;"">" + ..
               gettext("Version") + ..
               "</div>" + ..
               "<div>" + thisModuleDetails.Version  + "</div>" + ..
               authorHTML + ..
               "<div style=""font-weight:bold;margin-top:10px;margin-bottom:5px;"">" + ..
               gettext("Description") + ..
               "</div>" + ..
               "<div>" + ..
               strcat(thisModuleDetails.Description,"<br>")  + ..
               "</div>" + ..
               sizeHTML + ..
               "</body>" + ..
               "</html>";

    // Update the main description
    set(Desc,"String",htmlcode);

    // Description title management
    // =========================================================================

    descFrameHTML    = thisModuleDetails.Title;

    descFramePos     = get(DescTitle,"Position");
    descFramePos(3)  = 300;

    set(DescTitle, "String"   , descFrameHTML );
    set(DescTitle, "Position" , descFramePos );

    // Buttons
    // =========================================================================

    // Tests for update available
    // --------------------------

    moduleVersion     = atomsGetMRVersion(thisModuleName);
    installedVersions = atomsGetInstalledVers(thisModuleName);
    canUpdate         = "off";

    for k=1:size(installedVersions,"*")
        if atomsVersionCompare(installedVersions(k), moduleVersion)==-1 then
            canUpdate = "on";
            break
        end
    end

    // Can be removed
    // --------------------------

    if atomsIsInstalled(thisModuleName) then
        canRemove = "on";
    else
        canRemove = "off";
    end

    // Can be installed
    // --------------------------

    if ~ atomsIsInstalled(thisModuleName) then
        canInstall = "on";
    else
        canInstall = "off";
    end

    // Update the buttons
    // --------------------------

    set(findobj("tag", "installButton"), "Enable", canInstall);
    set(findobj("tag", "updateButton") , "Enable", canUpdate );
    set(findobj("tag", "removeButton") , "Enable", canRemove );

endfunction

// =============================================================================
// atomsSize2human()
// =============================================================================

function human_str = atomsSize2human(size_str)

    size_int = strtod(size_str);

    if size_int < 1024 then
        human_str = string(size_int) + " " + gettext("Bytes");

    elseif size_int < 1024*1024 then
        human_str = string(round(size_int/1024)) + " " + gettext("KB");

    else
        human_str = string( round((size_int*10)/(1024*1024)) / 10 ) + " " + gettext("MB");

    end

endfunction

// =============================================================================
// show()
// =============================================================================

function show(tag)
    obj = findobj("tag",tag);
    set(obj,"Visible","On");
endfunction

// =============================================================================
// hide()
// =============================================================================

function hide(tag)
    obj = findobj("tag",tag);
    set(obj,"Visible","Off");
endfunction

// =============================================================================
// showHome()
// + Hide the detailed description of a module
// + Show the home page
// =============================================================================

function showHome()

    // Hide the Desc frame
    hide("DescFrame");
    hide("DescTitle");
    hide("Desc");
    hide("removeButton");
    hide("installButton");
    hide("updateButton");

    // Show the Home page
    show("HomeFrame");
    show("HomeTitle");
    show("HomeListbox");

endfunction

// =============================================================================
// showDesc()
// + Hide the home page
// + Show the detailed description of a module
// =============================================================================

function showDesc()

    // Show the Home page
    hide("HomeFrame");
    hide("HomeTitle");
    hide("HomeListbox");

    // Hide the Desc frame
    show("DescFrame");
    show("DescTitle");
    show("Desc");
    show("removeButton");
    show("installButton");
    show("updateButton");

endfunction
