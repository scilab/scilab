// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// display of the local installed toolboxes

function listToolboxes()
  rep = atomsToolboxDirectory()
  cd(rep)
  listLocal = ls()
  [n, m] = size(listLocal)
  disp("The local installed toolboxes are :")
  for i=1:n
    if listLocal(i) <> ".svn"
      desc = atomsReadDesc(listLocal(i))
      disp(desc("Toolbox") + " - version " + desc("Version"))
    end
  end
endfunction
