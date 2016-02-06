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

function hdl=%h_set(varargin)
    if (lstsize(varargin) == 3)
        hdl = varargin(1)
        str = varargin(2)
        val = varargin(3)
        for i=1:size(hdl,"*")
            set(hdl(i),str,val)
        end,

    elseif (lstsize(varargin) == 2)
        str = varargin(1)
        val = varargin(2)
        hdl=[]
        set(str,val)
    end,
endfunction


