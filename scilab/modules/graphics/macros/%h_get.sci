// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function out_vect = %h_get(varargin)
    out_vect = [];
    if (lstsize(varargin) == 2)
        hdl = varargin(1)
        str = varargin(2)
        for i=1:size(hdl,'*')
            out_vect = [out_vect, get(hdl(i),str)]
        end

    elseif (lstsize(varargin) == 1)
        str = varargin(1)
        out_vect = get(str)
    end
endfunction
