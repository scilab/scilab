// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// If there are many values we split in some string

function fieldValue = atomsSplitValue(fieldValue, separate)
  index = strindex(fieldValue, separate)
  if length(index) > 0
    fieldValue = strsplit(fieldValue,index)
    // We remove the separator and the spaces which are in the field end
    fieldValue = strsubst(fieldValue, separate, "")
    fieldValue = strsubst(fieldValue, " ", "")
  end
endfunction
