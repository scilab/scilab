// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2012 - Cedric Delamarre
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x = %i_y_i(a, b)

    t1=inttype(a); t2=inttype(b);
    //logical promotions
    if t1<10 & t2<10 then t=max(t1,t2); end
    if t1>10 & t2>10 then t=max(t1,t2); end
    //more arbitrary promotions
    if t1<10 & t2>10 then t=max(t1,t2-10); end
    if t1>10 & t2<10 then t=max(t1-10,t2); end

    x = iconvert(double(a) ./. double(b), t);

endfunction
