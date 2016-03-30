// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
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

function flag=fseek_origin(origin)
    select origin
    case "bof" then
        flag="set"
    case "cof" then
        flag="cur"
    case "eof" then
        flag="end"
    case -1 then
        flag="set"
    case 0 then
        flag="cur"
    case 1 then
        flag="end"
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: %s, %s, %s, ''%s'', ''%s'' or ''%s'' expected.\n"),"mseek", 1, "-1", "0", "1", "bof", "cof", "eof"));
    end
endfunction
