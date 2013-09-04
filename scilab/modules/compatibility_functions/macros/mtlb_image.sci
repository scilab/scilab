// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2001 - INRIA - Serge STEER
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
