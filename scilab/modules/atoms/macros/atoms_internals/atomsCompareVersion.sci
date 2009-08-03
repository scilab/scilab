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

// Compare two versions ; returns 0 on equality, 1 if the first is greater
// than the second, -1 else

function result = atomsCompareVersion(v1, v2)
  if strtod(v1(1)) > strtod(v2(1))
    result = 1
  elseif strtod(v1(1)) < strtod(v2(1))
    result = -1
  else
    if size(v1, 1) >= 2 & size(v2, 1) >= 2
      result = atomsCompareVersion(v1(2:$), v2(2:$))
    else
      result = sign(size(v1, 1) - size(v2, 1))
    end
  end
endfunction
