// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge STEER
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function str=formatEvansTip(curve)
    //this function is called by the datatip mechanism to format the tip
    //string for the evans root loci curves
    ud = curve.parent.display_function_data;
    pt = curve.data(1:2);
    [d,ptp,i,c]=orthProj(curve.parent.data, pt);
    K=ud(i)+(ud(i+1)-ud(i))*c;
    str=msprintf(_("Pole: %.4g %+.4g i\nLoop gain: %.4g"), pt,K);
endfunction
