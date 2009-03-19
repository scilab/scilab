// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU <sylvestre.ledru@scilab.org>
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Building of the dependencies list 

function listeDep = atomsCheckConflict(name, version, checkVersionScilab)
  global conflictLocal
  global conflictingList
  global nomconflictLocal
  global conflictVersion
  // If there is no version
  if argn(2) == 1
    version = ""
  end
  clearglobal conflictingList
  listeDep = checkDependencies(name, version)
  [n, m] = size(listeDep)
  for i=1:n-1
    // We compare name
    [name1, version1] = atomsSeparateVersionDep(listeDep(i))
    [name2, version2] = atomsSeparateVersionDep(listeDep(i+1))
    if name1 == name2
      // If there are 2 possible versions, there is necessary a dependencies <=
      // If there is, this maximum version must match with the authers dep
      // We install in local, and if the conflictLocal tag is 1, there is a version conflict, else is ok
      v1 = atomsDecoupVersion(version1)
      v2 = atomsDecoupVersion(version2)
      if atomsCompareVersion(v1, v2) == 1 | atomsCompareVersion(v1, v2) == 0
        atomsDlInstall(name1, version2)
      else
        atomsDlInstall(name1, version1)
      end
      clearglobal conflictLocal
      clearglobal conflictingList
      clearglobal nomconflictLocal
      clearglobal conflictVersion
      listeDep2 = checkDependencies(name, version)
      if conflictLocal == 1
        conflictVersion = 1
        rep = atomsToolboxDirectory()
        rmdir(rep + name1, "s")
      end
    end
  end
endfunction

function listeDep = checkDependencies(name, version)
  // To avoid redundancies and loops
  global conflictingList
  global conflictLocal
  global nomconflictLocal
  global conflictVersion
  // If there is no version
  if argn(2) == 1
    version = ""
  end
  // Reading of the description file
  desc = atomsReadDesc(name)
  // Selection of the position toolbox in the disponible list toolboxes.
  position = atomsSelectPosition(desc, name, version, checkVersionScilab)
  // Case where the the toolbox is not present
  if position == 0
    atomsDisplayMessage(sprintf(_("Toolbox %s not found."),name))
    listeDep = ""
    return listeDep
  elseif position == 0.1
    conflictLocal = 1
    nomconflictLocal = name
    listeDep = ""
    return listeDep
  end
  // If the toolbox with this version have been already found, it's useless to build again the dependencies tree
  if find(conflictingList == (desc("Toolbox")(position) + " (" + desc("Version")(position) + ")"))
    resume
  end
  listeDep = desc("Toolbox")(position) + " (" + desc("Version")(position) + ")"
  [a, b] = size(conflictingList)
  conflictingList(a+1) = desc("Toolbox")(position) + " (" + desc("Version")(position) + ")"
  // Recuperation of the dependencies
  depends = atomsExtractValue("Depends", desc, position)
  // If there is no dependencies
  if depends == " " | depends == ""
  	return listeDep
  end
  // If there is many dependencies, we split it in many string
  depends = atomsSplitValue(depends, ',')
  // Addition in the toolboxes list to install
  [n, m] = size(depends)
  for i=1:n
	[nb, x] = size(listeDep)
	// Separation of the version and the dependencies
	[depend, version] = atomsSeparateVersionDep(depends(i))
	// Research of dependencies
	deps = checkDependencies(depend, version)
	[o, p] = size(deps)
	for i=1:o
      listeDep(nb+i)=deps(i)
    end	
  end
  listeDep = unique(listeDep)
  return listeDep
endfunction
