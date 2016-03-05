// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1998-2010 - INRIA - Serge Steer
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
