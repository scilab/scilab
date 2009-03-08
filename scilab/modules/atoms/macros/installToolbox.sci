// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Installation of a toolbox

function result = installToolbox(name, checkVersionScilab, version)
  global conflictingList
  global conflictLocal
  global nomconflictLocal
  global conflictVersion
  rhs = argn(2)
  if (rhs == 2 | rhs == 1 | rhs == 3) then
  // We check if the arguments exist
  if rhs == 1
    checkVersionScilab = %t
  end
  if rhs <= 2
    version = ""
  end
  // We remove the special characters
  name = atomsSubstituteString(name)
  // We go to the dedicated directory
  rep = atomsToolboxDirectory()
  cd (rep)
  // Required to empty the global variables before any use of atomsCheckConflict
  clearglobal conflictingList
  clearglobal conflictLocal
  clearglobal nomconflictLocal
  clearglobal conflictVersion
  // Retrieve of the list of toolboxes to install
  listTool = atomsCheckConflict(name, version, checkVersionScilab)
  // If there is an empty line, it is because a dependencie is missing
  if find(listTool == "") <> []
    if conflictLocal == 1
      atomsDisplayMessage(sprintf(_("The Toolbox version %s locally present doesn''t match. Update it.\n"),nomconflictLocal))
    elseif listTool <> ""
      atomsDisplayMessage(_("Impossible to install all the dependencies.\n"))
    end
    result =  %f
    return result
  elseif conflictVersion == 1
    atomsDisplayMessage(sprintf(_("Conflit of version for %s"),nomconflictLocal))
    result =  %f
    return result
  end
  // Retrieve of the toolboxes list which are already in local
  cd (rep)
  listLocal = ls()
  // Treatment of the toolboxes to install
  [m, n] = size(listTool)
  for i=1:m
    [name, version] = atomsSeparateVersionDep(listTool(i))
    // If it is locally present, we pass to the following one
    if find(listLocal == name) <> []
      continue
    else
      atomsDisplayMessage(_("Is going to be install: ") + name)
      if ~atomsDlInstall(name, version)
        disp(_("Error during the installation"))
        result = %f
        return result
      end
    end
  end
  result = %t
  return result

  else
    error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"installToolbox",1,3))
  end

endfunction
