// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Creation of a list packaging the informations of the DESCRIPTION file

function listDesc = atomsListDescription()
  [listeObl, listeOpt] = atomsConstant()
  [n, m] = size(listeObl)
  for i=1:m
    listDesc(listeObl(i))= []
  end
  [n, o] = size(listeOpt)
  for i=1:o
    listDesc(listeOpt(i))= []
  end
endfunction
