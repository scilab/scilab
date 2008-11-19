// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Remove of a toolbox

function result = removeToolbox(nom)
  rep = atomsToolboxDirectory()
  // If nom = "all" we delete all the Toolboxes
  if nom == "all"  
    rmdir(rep, "s")
    mkdir(rep)
  else
    // we verify the existence of the toolbox
    d = rep + nom
    if ~isdir(d)
      atomsDisplayMessage("The toolbox " + nom + " is not installed")
      result = %f
      return result
    end
    cd (rep)
    // We verify if the toolbox is not a dependancie of another toolbox
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
          if find(depend == nom) <> []
            repQuestion = atomsButtonYesNo("The Toolbox " + nom + " is a dependancie of " + listLocal(i) + ". Continue?")
            if (repQuestion == "2")
              result = %f
              return result
            end
          end
        end
      end
    end
    rmdir(rep + nom, "s")
    atomsDisplayMessage(nom + " delete")
    result = %t
    return result
  end
endfunction

function result = atomsButtonYesNo(message)
	if (getscilabmode() == "STD")
		result = buttondialog(message,"Yes|No","question")
	else
		response = input(message + " [""y""/""n""]")
		if (response <> "y" & response <> "Y")
			result = "2"
		else
			result = "1"
		end
	end
	return result
endfunction
