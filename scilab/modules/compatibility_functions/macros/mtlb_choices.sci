// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function mtlb_choices(nam,header,labels,callbacks,inter)

    while %t
        num=x_choose(labels,header,gettext("Close"))
        if num==0 then break,end
        execstr(callbacks(num))
    end

endfunction
