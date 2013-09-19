
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [al,be,Z]=gspec(A,B)
    //For backward compatibility
    warning(msprintf(gettext("%s: Obsolete function. Please replace ''%s'' by ''%s''."),"gspec","gspec","spec"));
    [al,be,Z]=spec(A,B)
endfunction
