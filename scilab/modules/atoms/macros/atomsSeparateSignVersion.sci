// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Separation of the sign >=, <= or =  of the version version

function [signe, version] = atomsSeparateSignVersion(version)
  index = strindex(version, "=")
  version = strsplit(version,index)
  signe = version(1)
  version = version(2)
endfunction
