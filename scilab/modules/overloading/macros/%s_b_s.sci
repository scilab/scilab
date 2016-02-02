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
function r=%s_b_s(varargin)
    //a:b:c where a and b are double
    if round(varargin(1))<>varargin(1)|round(varargin(2))<>varargin(2) then
        error(msprintf(_("%s: Wrong type for input arguments #%d and #%d: Integer values expected.\n"),"%s_b_s",1,2));
    end
    select type(varargin(3))
    case 8 then
        it=inttype(varargin(3))
        r=iconvert(varargin(1),it):iconvert(varargin(2),it):varargin(3)
    else
        error(msprintf(_("%s: Not yet implemented.\n"),"%s_b_s"));
    end
endfunction


