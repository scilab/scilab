// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  1985-2010 - INRIA - Serge Steer
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function str = formatBodeMagTip(datatipHandle)
    // This function is called by the datatips mechanism to format the tip
    // string for the magnitude bode curves
    pt = datatipHandle.data(1:2);
    str = msprintf("%.4g"+_("Hz")+"\n%.4g"+_("dB"), pt(1), pt(2));
endfunction
