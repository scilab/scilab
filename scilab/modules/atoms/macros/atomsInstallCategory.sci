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

// Installation of a toolbox sub-category  

function result = atomsInstallCategory(cat, sscat, checkVersionScilab)
  rhs = argn(2)

  if (rhs == 2 | rhs == 1 | rhs == 3) then
  if rhs == 2
    checkVersionScilab = %t
  end
  result = %f
  // we load the list of all the available toolboxes in the net
  desc = atomsReadDesc("")
  [nbTool, m] = size(desc("Toolbox"))
  for i=1:nbTool
    catTool = desc("Category")(i)
    // We watch if this toolbox belong to the asked cat/sscat
    regularExpression = "/" + cat + " \((\w*, )*" + sscat + "(, \w*)*\)/"
    // If yes, we install it if it is convenient for the Scilab version
    if regexp(catTool, regularExpression) <> [] & ((atomsVerifVersionScilab(desc("ScilabVersion")(i)) & checkVersionScilab) | ~checkVersionScilab)
      // We can't take result = atomsInstall() there is a install error, result = %f
      atomsInstall(desc("Toolbox")(i));
      result = %t
    end   
  end
  // If we have installed nothing
  if ~result
    atomsDisplayMessage(_("None of the toolboxes match in this category/sub-category for your version"))
  end
  return result
  else
    error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsInstallCategory",1,3))
  end
endfunction
