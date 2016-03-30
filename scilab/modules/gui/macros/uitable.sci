// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Han Dong
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
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
