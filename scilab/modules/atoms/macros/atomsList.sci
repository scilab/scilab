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

// display of the local installed toolboxes

function atomsList()
  rep = atomsToolboxDirectory()
  cd(rep)
  listLocal = ls()
  [n, m] = size(listLocal)
  disp(_("The local installed toolboxes are:"))
  for i=1:n
	desc = atomsReadDesc(listLocal(i))
	disp(desc("Toolbox") + " - version " + desc("Version"))
  end
endfunction
