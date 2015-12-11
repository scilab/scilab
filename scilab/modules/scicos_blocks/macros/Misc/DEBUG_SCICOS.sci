//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014-2014 - Scilab Enterprises - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
