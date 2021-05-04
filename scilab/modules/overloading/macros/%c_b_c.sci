// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2021 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r = %c_b_c(varargin)
    if  size(varargin)==2 then
        if type(varargin(1))==10 & type(varargin(2))==10 then
            s = ascii(varargin(1))
            e = ascii(varargin(2))
            if s>e, step = -1, else step = 1, end
            r = ascii(s:step:e)
        end
    else
        msg = _("%s: Non implemented feature.\n");
        error(msprintf(msg, "char:char"));
    end
endfunction
//assert_checkequal("a":"z", "abcdefghijklmnopqrstuvwxyz");
//assert_checkequal("z":"a", "zyxwvutsrqponmlkjihgfedcba");

