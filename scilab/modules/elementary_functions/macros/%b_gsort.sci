// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = %b_gsort(varargin)
    // Boolean hypermatrices are completely processed in %hm_gsort

    b = iconvert(varargin(1), 1);
    if argn(1)==1 then
        b = gsort(b, varargin(2:$))==int8(1);
        varargout = list(b);
    else
        [b, k] = gsort(b, varargin(2:$));
        b = (b == int8(1));
        varargout = list(b, k);
    end
endfunction
