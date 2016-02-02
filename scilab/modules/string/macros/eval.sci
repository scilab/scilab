// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
// Copyright (C) DIGITEO - 2010 - Allan CORENT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function _h = eval(z)
    // Syntax :  H = eval(Z)
    // returns the evaluation of the matrix of character strings Z.
    // Example: a=1; b=2; Z=['a','b'] ; eval(Z) returns the matrix [1,2];
    //!

    _h = [];
    [mz, nz] = size(z);
    if mz * nz > 1 then
        str = [];
        for l = 1:mz, for k = 1:nz,
                str = [str; "%_h(" + string(l) + ", " + string(k) + ")=" + z(l,k)];
            end
        end
    else
        str = "%_h = " + z;
    end
    %_h = [];
    deff("[%_h] = %eval();", str);
    _h = %eval();
endfunction
