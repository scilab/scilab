// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Display of the toolbox information

function infoToolbox(nom)
  // we remove the special characters
  nom = atomsSubstituteString(nom)
  // We go on the toolboxes repertory
  rep = atomsToolboxDirectory()
  d = rep + nom
  // Gestion of the different OS
  if getos() == "Windows"
    directory = d + "\DESCRIPTION"
  else // linux and mac
    directory = d + "/DESCRIPTION"
  end
  // If we find the repertory in local and the DESCRIPTION file is present
  if (isdir(d) & ls(directory) <> [])
    desc = atomsReadDesc(nom)
    functionTool = desc("Function")
    disp(_("This toolbox is present locally."))
    disp(desc)
    disp(functionTool)
  // Else we search in the net
  else
    disp(_("Toolbox versions available in the web"))
    listDesc = atomsReadDesc("")
    versions = ""
    [n, m] = size(listDesc("Toolbox"))
    for i=1:n
      if listDesc("Toolbox")(i) == nom
        // To avoid version redundancy
        [a, b] = size(versions)
        if find(versions == listDesc("Version")(i))
          continue
        else
          versions(a+1) = listDesc("Version")(i)
          desc = atomsListDescription()
          [listeObl, listeOpt] = constant()
          [o, p] = size(listeOpt)
          [n, m] = size(listeObl)
          for j=1:m
            desc(listeObl(j)) = listDesc(listeObl(j))(i)
          end
          for j=1:p
            desc(listeOpt(j)) = listDesc(listeOpt(j))(i)
          end
          disp(desc)
        end
      end
    end
    // if no version was found
    [a, b] = size(versions)
    if a == 1
     disp("none")
    end
  end
endfunction
