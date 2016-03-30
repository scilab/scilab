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
function M1=%hm_3_hm(M1,M2)
    if and(M1.dims==M2.dims)==%f then
        error(msprintf(_("%s: Input argument #%d and input argument #%d have incompatible dimensions.\n"),"%hm_3_hm",1,2));
    end
    M1.entries=(M1.entries>=M2.entries)
endfunction
