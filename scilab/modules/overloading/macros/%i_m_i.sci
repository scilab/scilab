// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function i1=%i_m_i(i1,i2)
    t1=inttype(i1); t2=inttype(i2);
    //logical promotions
    if t1<10 & t2<10 then t=max(t1,t2); end
    if t1>10 & t2>10 then t=max(t1,t2); end
    //more arbitrary promotions
    if t1<10 & t2>10 then t=max(t1+10,t2); end
    if t1>10 & t2<10 then t=max(t1,t2+10); end
    //  i1=iconvert(i1,t) * iconvert(i2,t)
    i1 = iconvert(double(i1) * double(i2), t);
endfunction
