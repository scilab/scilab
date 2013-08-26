// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Han Dong
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function h=uitable(varargin)

    Rhs = length(varargin);

    //creates a preliminary uitable
    h = uicontrol("style", "uitable");

    if Rhs > 0 then
        //sets the propertyName with its propertyValue
        for i=1:2:Rhs
            propertyName = varargin(i);
            propertyValue = varargin(i+1);
            set(h, propertyName, propertyValue);
        end
    end
endfunction
//-----------------------------------------------------------------------------
