// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function []=halt(varargin)

    //halt() stops execution until something is entered in the keyboard.

    [lhs,rhs] = argn(0);

    msg = "halt";

    if (rhs > 0) then

        if rhs > 1  then
            error(msprintf(gettext("%s: Wrong number of input argument.\n"),"halt"));
        end

        if type(varargin(1)) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"halt",1));
        end

        if size(varargin(1),"*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"halt",1));
        end

        msg = string(varargin(1));
    end

    mprintf(msg);
    mscanf("%c");

endfunction
