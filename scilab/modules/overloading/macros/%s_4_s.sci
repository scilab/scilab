// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0, pursuant
// to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1, and
// continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function TF = %s_4_s(A, B)
    // Possible case: all components of both A and B have both strictly null
    // imaginary parts
    if isreal(A, 0) & isreal(B,0) then
        TF = real(A) >= real(B)
    end
endfunction
