// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Recuperation of the Mirror list

function listMirror = atomsToolboxMirror()
  if getos() == "Windows"
    listMirror = ["http://128.93.23.238/scilab/bin/windows/contrib"]
  else // linux and mac
    listMirror = ["http://128.93.23.238/scilab/src/contrib"]
  end
endfunction
