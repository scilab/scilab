// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Selection of the toolboxes corresponding at the good scilab version

function result = atomsVerifVersionScilab(scilabVersion)
  // we get back the software version
  currentVersion = getversion()
  index = strindex(currentVersion, "-")
  currentVersion = strsplit(currentVersion,index)
  [a, b]=size(index)
  // If there is more than one "-" it is trunk or a alpha/beta/rc
  if b == 3 
    modifCurrentVersion = currentVersion(3) + currentVersion(4) 
  end
  currentVersion = currentVersion(2)
  currentVersion = strsubst(currentVersion, "-", "")
  // We shape the toolbox scilab version
  scilabVersion = strsubst(scilabVersion, " ", "")
  [signeScilab, versionScilab] = atomsSeparateSignVersion(scilabVersion)
  // We check also here if it's a alpha/beta/rc
  index = strindex(versionScilab, "-")
  if index <> []
    versionScilab = strsplit(versionScilab,index)
    modifVersionScilab = versionScilab(2) + versionScilab(3)
    versionScilab = versionScilab(1)
    versionScilab = strsubst(versionScilab, "-", "")
  end
  // We check the different versions to know if the versions match
  if currentVersion == "trunk"
    if signeScilab == ">="
      result = %T
    else
      result = %F
    end
  else
    if atomsCompareVersion(atomsDecoupVersion(versionScilab), atomsDecoupVersion(currentVersion)) == 0
      // We check if we have different versions
      if isdef("modifCurrentVersion") & isdef("modifVersionScilab")
        if verifVersionModif(modifCurrentVersion, modifVersionScilab) == 0
          result = %T
        elseif signeScilab == "<=" & verifVersionModif(modifCurrentVersion, modifVersionScilab) == -1
          result = %T
        elseif signeScilab == ">=" & verifVersionModif(modifCurrentVersion, modifVersionScilab) == 1
          result = %T
        else
          result = %F
        end
      elseif isdef("modifCurrentVersion") & ~isdef("modifVersionScilab") & signeScilab == "<="
        result = %T
      elseif ~isdef("modifCurrentVersion") & isdef("modifVersionScilab") & signeScilab == ">="
        result = %T
      else
        result = %F
      end
    elseif signeScilab == "<=" & atomsCompareVersion(atomsDecoupVersion(versionScilab), atomsDecoupVersion(currentVersion)) == 1
      result = %T
    elseif signeScilab == ">=" & atomsCompareVersion(atomsDecoupVersion(versionScilab), atomsDecoupVersion(currentVersion)) == -1
      result = %T
    else
      result = %F
    end
  end
endfunction

function result = verifVersionModif(vScilab, vTool)
  index = strindex(vScilab, "-")
  vScilab = strsplit(vScilab,index)
  index = strindex(vTool, "-")
  vTool = strsplit(vTool,index)
  // We compare versions : 1 if vScilab > vTool; 0 if vScilab = vTool; -1 if vScilab < vTool
  // alpha < beta < rc
  if length(vScilab(1)) > length(vTool(1))
    return -1
  elseif length(vScilab(1)) < length(vTool(1))
    return 1
  else
    if strtod(vScilab(2)) > strtod(vTool(2))
      return 1
    elseif strtod(vScilab(2)) < strtod(vTool(2))
      return -1
    else
      return 0
    end
  end
endfunction


