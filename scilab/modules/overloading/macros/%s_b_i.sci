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
function r=%s_b_i(varargin)
    //a:b where a is double and b int
    //a:b:c where a is double,  b int and c every type
    if round(varargin(1))<>varargin(1) then
        error(msprintf(_("%s: Wrong type for input argument #%d: integer values expected.\n"),"%s_b_i",1));
    end
    if size(varargin)==2 then //a:b
        it=inttype(varargin(2))
        r=iconvert(varargin(1),it):varargin(2)
    else
        it=inttype(varargin(2))
        r=iconvert(varargin(1),it):varargin(2):iconvert(varargin(3),it)
    end
endfunction


