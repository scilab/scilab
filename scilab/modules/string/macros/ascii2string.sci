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


function ww=ascii2string(w1)
    //converts a numeric (real or integer) matrix made of ascii codes
    //into a one column scilab string matrix and conversely.
    ww=[];
    for k=1:size(w1,1)
        ww=[ww;ascii(w1(k,:))];
    end
endfunction
