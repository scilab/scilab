// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2001 - INRIA - Serge STEER
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function h=mtlb_image(varargin)
    h=xget("window")
    //find first propertyname position
    Rhs=size(varargin)
    for k=1:Rhs
        if type(varargin(k))==10 then
            Rhs=k-1
            break,
        end
    end
    //Only C argument taken into account.
    C=varargin(Rhs)
    Matplot(C)
endfunction
