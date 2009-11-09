// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

function cbAtomsGui()

    // Load Atoms Internals lib if it's not already loaded
    if ~ exists("atomsinternalslib") then
        load("SCI/modules/atoms/macros/atoms_internals/lib");
    end
    
    if get(gcbo, "Tag") == "modulesListbox" then // Display selected module infos
        updateAtomsGui();
    
    // =========================================================================
    // Install selected module
    // =========================================================================
    
    elseif  get(gcbo, "Tag") == "installButton" then 
       
        disableAtomsGui();
        set(findobj("Tag", "modulesDesc"), "String", gettext("Installing..."));
    
        if execstr("atomsInstall(getSelectedModuleName())", "errcatch")<>0 then
            messagebox(gettext("Installation failed !"), gettext("Atoms error"), "error");
        else
            messagebox(gettext("Installation done !"), gettext("Atoms"), "info");
        end
    
        updateAtomsGui();
    
    // =========================================================================
    // Remove selected module
    // =========================================================================
    
    elseif  get(gcbo, "Tag") == "removeButton" then // Remove selected module

        disableAtomsGui();
        
        set(findobj("Tag", "modulesDesc"), "String", gettext("Removing..."));

        if execstr("atomsRemove(getSelectedModuleName())", "errcatch")<>0 then
            messagebox(gettext("Remove failed !"), gettext("Atoms error"), "error");
        else
            messagebox(gettext("Remove done !"), gettext("Atoms"), "info");
        end

        updateAtomsGui();
    
    // =========================================================================
    // Update selected module
    // =========================================================================
    
    elseif  get(gcbo, "Tag") == "updateButton" then // Update selected module
    
        disableAtomsGui();

        set(findobj("Tag", "modulesDesc"), "String", gettext("Updating..."));
        
        if execstr("atomsRemove(getSelectedModuleName())", "errcatch")<>0 then
            messagebox(gettext("Update failed !"), gettext("Atoms error"), "error");
        else
            messagebox(gettext("Update done !"), gettext("Atoms"), "info");
        end

        updateAtomsGui();
    
    // =========================================================================
    // Close menu
    // =========================================================================
    
    elseif  get(gcbo, "Tag") == "closeAtomsMenu" then
        delete(findobj("Tag", "atomsFigure"));

   
    // =========================================================================
    // Help menu
    // =========================================================================
    
    elseif  get(gcbo, "Tag") == "helpAtomsMenu" then

        help("atoms")

    end

endfunction

function modulename = getSelectedModuleName()
    // Get selected module
    selected = get(findobj("Tag", "modulesListbox"), "Value");

    allModules = get(findobj("Tag", "atomsFigure"), "userdata");
    modulesNames = getfield(1, allModules);
    modulesNames (1:2) = [];

    modulename = modulesNames(selected); 
endfunction


function disableAtomsGui()
    set(findobj("tag", "installButton"), "Enable", "off");
    set(findobj("tag", "updateButton"), "Enable", "off");
    set(findobj("tag", "removeButton"), "Enable", "off");
    set(findobj("tag", "modulesListbox"), "Enable", "off");
endfunction

function updateAtomsGui()

    set(findobj("tag", "modulesListbox"), "Enable", "on");

    // Get selected module
    modulesListbox = findobj("tag", "modulesListbox")
    selected = get(modulesListbox, "Value");

    // Get the modules list
    allModules = get(findobj("Tag", "atomsFigure"), "userdata");

    // Update the description

    modulesNames       = getfield(1, allModules);
    modulesNames (1:2) = [];
    themodule          = allModules(getSelectedModuleName());
    vers               = getfield(1, themodule);
    descZone           = findobj("tag", "modulesDesc");
    descFrameTitle     = findobj("tag", "modulesDescFrameTitle");

    htmlcode           = "<html>" + ..
                        "<body>" + ..
                        "<div style=""font-weight:bold;margin-top:10px;margin-bottom:5px;"">" + ..
                        "  Version" + ..
                        "</div>" + ..
                        "<div>" + themodule(vers(3)).Version  + "</div>" + ..
                        "<div style=""font-weight:bold;margin-top:10px;margin-bottom:5px;"">" + ..
                        "  Description" + ..
                        "</div>" + ..  
                        "<div>" + ..
                        strcat(themodule(vers(3)).Description,"<br>")  + ..
                        "</div>" + ..
                        "</body>" + ..
                        "</html>";


    set(descZone, "String", htmlcode);



    descFrameHTML    = "<html>" + ..
                        "<body>" + ..
                        "<div style=""font-weight:bold;margin-top:10px;margin-bottom:5px;font-size:105%;"">" + themodule(vers(3)).Title + "</div>" + ..
                        "</body>" + ..
                        "</html>";

   
    descFramePos     = get(descFrameTitle, "Position");
    descFramePos(3)  = 300;
    
    set(descFrameTitle, "String"   , descFrameHTML );
    set(descFrameTitle, "Position" , descFramePos );
    
    // Tests for update available
    moduleVersion     = atomsGetMRVersion(getSelectedModuleName());
    installedVersions = atomsGetInstalledVers(getSelectedModuleName());
    canUpdate         = "off";
    
    for k=1:size(installedVersions,"*")
        if atomsVersionCompare(installedVersions(k), moduleVersion)==-1 then
            canUpdate = "on";
            break
        end
    end

    if atomsIsInstalled(getSelectedModuleName()) then
        canRemove = "on";
    else
        canRemove = "off";
    end

    if ~ atomsIsInstalled(getSelectedModuleName()) then
    canInstall = "on";
    else
    canInstall = "off";
    end

    set(findobj("tag", "installButton"), "Enable", canInstall );
    set(findobj("tag", "updateButton") , "Enable", canUpdate );
    set(findobj("tag", "removeButton") , "Enable", canRemove );

endfunction
