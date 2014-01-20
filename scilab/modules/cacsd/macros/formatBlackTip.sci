// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1998-2010 - INRIA - Serge Steer
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function str=formatBlackTip(datatipHandle)
    //This function is called by the datatip mechanism to format the tip
    //string for black curves.
    ud = datatipHandle.parent.display_function_data;
    pt = datatipHandle.data(1:2);
    [d,ptp,i,c]=orthProj(datatipHandle.parent.data, pt);
    disp(c)
    f=ud(i)+(ud(i+1)-ud(i))*c;
    str=msprintf("%.4g°\n%.4g"+_("dB")+"\n%.4g"+_("Hz"), pt,f);

endfunction
