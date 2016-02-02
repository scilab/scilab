// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function out_vect = %h_get(varargin)
    out_vect = [];
    if (lstsize(varargin) == 2)
        hdl = varargin(1)
        str = varargin(2)
        for i=1:size(hdl,"*")
            out_vect = [out_vect, get(hdl(i),str)]
        end

    elseif (lstsize(varargin) == 1)
        str = varargin(1)
        out_vect = get(str)
    end
endfunction
