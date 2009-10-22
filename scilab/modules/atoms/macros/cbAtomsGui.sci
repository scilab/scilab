// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function cbAtomsGui()

// Load Atoms Internals lib if it's not already loaded
if ~ exists("atomsinternalslib") then
  load("SCI/modules/atoms/macros/atoms_internals/lib");
end

if get(gcbo, "Tag") == "modulesListbox" then // Display selected module infos
  
  updateAtomsGui();
  
elseif  get(gcbo, "Tag") == "installButton" then // Install selected module
  
  disableAtomsGui();
  
  set(findobj("Tag", "modulesDesc"), "String", gettext("Installing..."));
  
  if execstr("atomsInstall(getSelectedModuleName())", "errcatch")<>0 then
    
    messagebox(gettext("Installation failed !"), gettext("Atoms error"), "error");
  
  else
  
    messagebox(gettext("Installation done !"), gettext("Atoms"), "info");

  end

  updateAtomsGui();
  
elseif  get(gcbo, "Tag") == "removeButton" then // Remove selected module

  disableAtomsGui();
  
  set(findobj("Tag", "modulesDesc"), "String", gettext("Removing..."));

  if execstr("atomsRemove(getSelectedModuleName())", "errcatch")<>0 then
    messagebox(gettext("Remove failed !"), gettext("Atoms error"), "error");
  else
    messagebox(gettext("Remove done !"), gettext("Atoms"), "info");
  end

  updateAtomsGui();
  
elseif  get(gcbo, "Tag") == "updateButton" then // Update selected module
  
  disableAtomsGui();

  set(findobj("Tag", "modulesDesc"), "String", gettext("Updating..."));
  
  if execstr("atomsRemove(getSelectedModuleName())", "errcatch")<>0 then
    messagebox(gettext("Update failed !"), gettext("Atoms error"), "error");
  else
    messagebox(gettext("Update done !"), gettext("Atoms"), "info");
  end

  updateAtomsGui();
  
elseif  get(gcbo, "Tag") == "closeAtomsMenu" then

  delete(findobj("Tag", "atomsFigure"));

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
modulesNames = getfield(1, allModules);
modulesNames (1:2) = [];
themodule = allModules(getSelectedModuleName());
vers = getfield(1, themodule);
descZone = findobj("tag", "modulesDesc");
set(descZone, "String", "<HTML><BODY>"+strcat(themodule(vers(3)).Description,"<BR>") + "</BODY></HTML>");

// Tests for update available
moduleVersion = atomsGetMRVersion(getSelectedModuleName());
installedVersions = atomsGetInstalledVers(getSelectedModuleName());
canUpdate = %F;
for k=1:size(installedVersions,"*")
  if atomsCompareVersion(installedVersions(k), moduleVersion)==-1 then
    canUpdate = %T;
    break
  end
end

if canUpdate then
  set(findobj("tag", "installButton"), "Enable", "off");
  set(findobj("tag", "updateButton"), "Enable", "on");
  set(findobj("tag", "removeButton"), "Enable", "on");
elseif atomsIsInstalled(getSelectedModuleName()) then
  set(findobj("tag", "installButton"), "Enable", "off");
  set(findobj("tag", "updateButton"), "Enable", "off");
  set(findobj("tag", "removeButton"), "Enable", "on");
else
  set(findobj("tag", "installButton"), "Enable", "on");
  set(findobj("tag", "updateButton"), "Enable", "off");
  set(findobj("tag", "removeButton"), "Enable", "off");
end

endfunction



