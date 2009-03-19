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

// End user function

// update of a toolbox

function result = atomsUpdate(name, checkVersionScilab)
  rhs = argn(2)

  if (rhs == 2 | rhs == 1) then
  // We check if the second argument exists
  if rhs == 1 then
    checkVersionScilab = %t
  end
  // We remove special characters
  name = atomsSubstituteString(name)
  // if name = "all" we update all the Toolboxes
  if name == "all"
    listLocal = ls()
    [n, m] = size(listLocal)
    for k=1:n
      if listLocal(i) <> ".svn"
        if ~atomsUpdate(listLocal(k))
          break
        end
      end
    end
  else
    // We check if the Toolbox locally exist
    rep = atomsToolboxDirectory()
    d = rep + name
    if ~isdir(d)
      atomsDisplayMessage(sprintf(_("The toolbox %s is not installed.\n")),name)
      result = %f
      return result
    end
    // We get back the current version 
    desc = atomsReadDesc(name)
    versionActuelle = atomsExtractValue("Version", desc, 1)
    versionActuelle = atomsDecoupVersion(versionActuelle)
    // We get back the list of all the other available toolboxes 
    listDesc = atomsReadDesc("")
    // We take the more recent version
    position = atomsSelectPosition(listDesc, name, "", checkVersionScilab)
    // If the toolbox is not available online
    if position == 0
      atomsDisplayMessage(_("Toolbox not available."))
      result = %f
      return result
    end
    versionNew = atomsExtractValue("Version", listDesc, position)
    versionNew = atomsDecoupVersion(versionNew)
    // We check if it is an update of the local
    if atomsCompareVersion(versionNew, versionActuelle) == 1
      // We check if it was a max dependency for other toolboxes
      listLocal = ls()
      [n, m] = size(listLocal)
      for i=1:n
        if listLocal(i) <> ".svn"
          desc = atomsReadDesc(listLocal(i))
          depends = atomsExtractValue("Depends", desc, 1)
          depends = atomsSplitValue(depends, ",")
          [n, m] = size(depends)
          for j=1:n
            [depend, version] = atomsSeparateVersionDep(depends(j))
            if version <> ""
              [signe, version] = atomsSeparateSignVersion(version)
              if find(depend == name) <> [] & signe == "<="
                version = atomsDecoupVersion(version)
                // We check if the new version confirm this max dependency
                if atomsCompareVersion(versionNew, version) == 1
                  atomsDisplayMessage(_("Dependency issue.\n"))
                  result = %f
                  return result
                end
              end
            end
          end
        end
      end
    end
    // We verify if it is necessary to update the dependencies
    dependsNew = atomsExtractValue("Depends", listDesc, position)
    dependsNew = atomsSplitValue(dependsNew, ",")
    [n, m] = size(dependsNew)
    for i=1:n
      [dependsNew, versionNew] = atomsSeparateVersionDep(dependsNew(i))
      listLocal = ls()
      if find(listLocal == dependsNew)
        if versionNew <> ""
          desc = atomsReadDesc(listLocal(i))
          v1 = atomsExtractValue("Version", desc, 1)
          v1 = atomsDecoupVersion(v1)
          [signeNew, versionNew] = atomsSeparateSignVersion(versionNew)
          versionNew = atomsDecoupVersion(versionNew)
          if ((versionNew == "<=" | versionNew == "=")  & atomsCompareVersion(v1, versionNew) == -1) | ((versionNew == ">=" | versionNew == "=") & atomsCompareVersion(v1, versionNew) == 1) 
            atomsUpdate(dependsNew);
          end
        end
      end
    end
    // We warm it
    versionNew = atomsExtractValue("Version", listDesc, position)
    atomsDisplayMessage(sprintf(_("The toolbox %s is going to be updated to version %s.\n"),name,versionNew))
    // We install the new version
    rmdir(rep + name, "s")
    atomsInstall(name);
  end
  result = %t
  return result

  else
    error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsUpdate",1,2))
  end

endfunction
