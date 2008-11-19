// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Building of the dependancies list 

function listeDep = atomsCheckConflict(nom, version, checkVersionScilab)
  global conflictLocal
  global conflictingList
  global nomconflictLocal
  global conflictVersion
  // If there is no version
  if argn(2) == 1
    version = ""
  end
  clearglobal conflictingList
  listeDep = checkDependencies(nom, version)
  [n, m] = size(listeDep)
  for i=1:n-1
    // We compare name
    [nom1, version1] = atomsSeparateVersionDep(listeDep(i))
    [nom2, version2] = atomsSeparateVersionDep(listeDep(i+1))
    if nom1 == nom2
      // If there are 2 versions possible, there is necessary a dependancies <=
      // If there is, this maximum version must match with the authers dep
      // We install in local, and if the conflictLocal tag is 1, there is a version conflict, else is ok
      v1 = atomsDecoupVersion(version1)
      v2 = atomsDecoupVersion(version2)
      if atomsCompareVersion(v1, v2) == 1 | atomsCompareVersion(v1, v2) == 0
        atomsDlInstall(nom1, version2)
      else
        atomsDlInstall(nom1, version1)
      end
      clearglobal conflictLocal
      clearglobal conflictingList
      clearglobal nomconflictLocal
      clearglobal conflictVersion
      listeDep2 = checkDependencies(nom, version)
      if conflictLocal == 1
        conflictVersion = 1
        rep = atomsToolboxDirectory()
        rmdir(rep + nom1, "s")
      end
    end
  end
endfunction

function listeDep = checkDependencies(nom, version)
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
  desc = atomsReadDesc(nom)
  // Selection of the position toolbox in the disponible list toolboxes.
  position = atomsSelectPosition(desc, nom, version, checkVersionScilab)
  // Case where the the toolbox is not present
  if position == 0
    atomsDisplayMessage("Toolbox " + nom + " not find")
    listeDep = ""
    return listeDep
  elseif position == 0.1
    conflictLocal = 1
    nomconflictLocal = nom
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
  // Recuperation of the dependancies
  depends = atomsExtractValue("Depends", desc, position)
  // If there is no dependancies
  if depends == " " | depends == ""
  	return listeDep
  end
  // If there is many dependancies, we split it in many string
  depends = atomsSplitValue(depends, ',')
  // Addition in the toolboxes list to install
  [n, m] = size(depends)
  for i=1:n
	[nb, x] = size(listeDep)
	// Separation of the version and the dependancie
	[depend, version] = atomsSeparateVersionDep(depends(i))
	// Research of dependancies
	deps = checkDependencies(depend, version)
	[o, p] = size(deps)
	for i=1:o
      listeDep(nb+i)=deps(i)
    end	
  end
  listeDep = unique(listeDep)
  return listeDep
endfunction
