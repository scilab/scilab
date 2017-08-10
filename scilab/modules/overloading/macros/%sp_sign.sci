// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - 2016 - Samuel GOUGEON
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Beware about the bug http://bugzilla.scilab.org/14460

function r = %sp_sign(m)
    r = m
    c = find(imag(m)~=0)
    if c~=[] then
        r(c) = m(c) ./ abs(m(c))
    end

    re = real(m)
    im = imag(m)
    r(find(im==0 & re>0)) = 1
    r(find(im==0 & re<0)) = -1
endfunction
