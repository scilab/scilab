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

// Display of the toolbox information

function atomsInfo(nameToolbox)

  rhs=argn(2);

  if rhs == 1 then
  // we remove the special characters
  nameToolbox = atomsSubstituteString(nameToolbox)
  // We go on the toolboxes repertory
  rep = atomsToolboxDirectory()
  d = rep + nameToolbox
  // Gestion of the different OS
  if getos() == "Windows"
    directory = d + "\DESCRIPTION"
  else // linux and mac
    directory = d + "/DESCRIPTION"
  end
  // If we find the directory in local and the DESCRIPTION file is present
  if (isdir(d) & ls(directory) <> [])
    desc = atomsReadDesc(nameToolbox)
    functionTool = desc("Function")
    disp(_("This toolbox is present locally."))
    disp(desc)
    disp(functionTool)
  // Else we search in the net
  else
    disp(_("Search for toolbox versions available on the web"))
    listDesc = atomsReadDesc("")
    versions = ""
    [n, m] = size(listDesc("Toolbox"))
    for i=1:n
      if listDesc("Toolbox")(i) == nameToolbox
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
  else
    error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsInfo",1));
  end

endfunction
