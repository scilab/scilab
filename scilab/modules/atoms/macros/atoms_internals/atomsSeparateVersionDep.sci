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

// Internal function

// Separation of the dependencies and the version

function [depend, version] = atomsSeparateVersionDep(depend)
  version = ""
  index = strindex(depend, "(")
  if index > 0
    // index-1 to keep the ( with the version
    temp = strsplit(depend,index-1) 
    depend = temp(1)
    version = temp(2)
    // we remove the brackets and the spaces
    version = strsubst(version, "(", "")
    version = strsubst(version, ")", "")
    version = strsubst(version, " ", "")
    depend = strsubst(depend, " ", "")
  end
endfunction
