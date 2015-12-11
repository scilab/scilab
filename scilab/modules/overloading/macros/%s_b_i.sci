// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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


