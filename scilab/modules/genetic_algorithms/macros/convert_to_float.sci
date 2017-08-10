// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = convert_to_float(code, binlen, maxbound, minbound)
    Max_Bin = 2^binlen-1;
    y       = [];
    for i=1:length(maxbound)
        y(i) = (maxbound(i) - minbound(i)) * (bin2dec(part(code,(i-1)*binlen+1:i*binlen)) / Max_Bin) + minbound(i);
    end
endfunction
