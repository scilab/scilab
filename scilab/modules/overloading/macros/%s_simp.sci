// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) Samuel GOUGEON - 2015 - sgougeon@free.fr : Bug 13893
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [num,den] = %s_simp(num,den)
    // implement complex case
    // 0/den => 0/1
    i = find(clean(num)==0 & clean(den)~=0)
    den(i) = 1
endfunction
