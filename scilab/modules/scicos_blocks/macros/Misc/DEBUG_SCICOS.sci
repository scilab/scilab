//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014-2014 - Scilab Enterprises - Bruno JOFRET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

function [x,y,typ]=DEBUG_SCICOS(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
        case "set" then
            arg1.gui = "DEBUG"
            [x,y,typ] = DEBUG("set", arg1);
        case "define" then
            x = DEBUG("define")
    end
endfunction
