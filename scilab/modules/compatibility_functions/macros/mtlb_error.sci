// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Serge STEER
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function mtlb_error(varargin)

    if argn(2)==1 then
        m=varargin(1)
    else
        F=varargin(1)
        //check if it is an identifier or a message
        //if strindex(F,'%')==[] then //not a format
        if size(tokens(F,":"),"*")>1 then
            // an identifier, it is ignored
            m=msprintf(varargin(2:$))
        else
            m=msprintf(varargin(:))
        end
        //end
    end
    error(m)
endfunction
