// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Division of the version in 2 or 3 numbers

function version = atomsDecoupVersion(version)
  index = strindex(version, ".")
  version = strsplit(version,index)
  version = strsubst(version, ".", "")
  version = strsubst(version, " ", "")
endfunction
