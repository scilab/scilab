// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2015  - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//=============================================================================
function ret = dlwIsExpress()
    ret = %f;
    if  dlwIsVc10Express() | ...
        dlwIsVc11Express() | ...
        dlwIsVc12Express() | ...
        dlwIsVc14Express() then
        ret = %t;
    end
endfunction